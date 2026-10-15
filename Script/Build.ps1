<# @brief 根据项目的引擎关联构建目标；支持显式指定引擎目录以兼容未注册的安装。 #>
param(
    [ValidateSet("Game", "Editor", "Client", "Server")]
    [string]$Target = "Game",

    [ValidateSet("Development", "Debug", "Shipping", "DebugGame", "Test")]
    [string]$Configuration = "Development",

    [ValidateSet("Win64", "Linux", "Mac")]
    [string]$Platform = "Win64",

    [switch]$Clean,
    [switch]$Run,

    <# @brief 可选的引擎根目录；为空时按项目版本查找注册表、Launcher 清单和常用路径。 #>
    [string]$EnginePath
)

$ErrorActionPreference = "Stop"
$ScriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$ProjectRoot = Resolve-Path "$ScriptRoot\.."
$ProjectFile = Get-ChildItem -Path $ProjectRoot -Filter "*.uproject" | Select-Object -First 1

if (-not $ProjectFile) {
    Write-Error "No .uproject file found in $ProjectRoot"
    exit 1
}

$ProjectName = [System.IO.Path]::GetFileNameWithoutExtension($ProjectFile.Name)

Write-Host "========================================" -ForegroundColor Cyan
Write-Host " Build Configuration" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Project      : $ProjectName"
Write-Host "  Target       : $Target"
Write-Host "  Configuration: $Configuration"
Write-Host "  Platform     : $Platform"
Write-Host "  Project Root : $ProjectRoot"
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

<# @brief 以项目版本为准查找引擎，避免升级后构建脚本仍调用旧版 UBT。 #>
$EngineAssociation = (Get-Content -LiteralPath $ProjectFile.FullName -Raw | ConvertFrom-Json).EngineAssociation
<# @brief 按优先级收集候选目录，后续验证文件与版本，跳过已失效的注册表记录。 #>
$EngineCandidates = @()
$UEInstallDir = $null

if ($EnginePath) {
    $EngineCandidates += $EnginePath
}
else {
    foreach ($RegistryPath in @(
        "HKLM:\SOFTWARE\EpicGames\Unreal Engine\$EngineAssociation",
        "HKLM:\SOFTWARE\WOW6432Node\EpicGames\Unreal Engine\$EngineAssociation"
    )) {
        $EngineCandidates += (Get-ItemProperty -Path $RegistryPath -ErrorAction SilentlyContinue).InstalledDirectory
    }

    <# @brief Launcher 清单也能识别已安装但尚未写入引擎注册表的版本。 #>
    $LauncherManifest = Join-Path $env:ProgramData 'Epic\UnrealEngineLauncher\LauncherInstalled.dat'
    if (Test-Path -LiteralPath $LauncherManifest) {
        $Installations = (Get-Content -LiteralPath $LauncherManifest -Raw | ConvertFrom-Json).InstallationList
        $EngineCandidates += $Installations |
            Where-Object { $_.AppName -eq "UE_$EngineAssociation" } |
            ForEach-Object { $_.InstallLocation }
    }

    $EngineCandidates += @(
        "D:\UE\UE_$EngineAssociation",
        "C:\UE\UE_$EngineAssociation",
        "C:\Program Files\Epic Games\UE_$EngineAssociation",
        "D:\Program Files\Epic Games\UE_$EngineAssociation",
        "D:\Programs\Unreal\UE_$EngineAssociation"
    )
}

foreach ($Candidate in ($EngineCandidates | Where-Object { $_ } | Select-Object -Unique)) {
    if (-not (Test-Path -LiteralPath "$Candidate\Engine\Build\BatchFiles\Build.bat") -or
        -not (Test-Path -LiteralPath "$Candidate\Engine\Build\Build.version")) {
        continue
    }

    <# @brief 防止显式路径或残留安装记录将当前项目交给不同主次版本的引擎。 #>
    $InstalledVersion = Get-Content -LiteralPath "$Candidate\Engine\Build\Build.version" -Raw | ConvertFrom-Json
    if ("$($InstalledVersion.MajorVersion).$($InstalledVersion.MinorVersion)" -eq $EngineAssociation) {
        $UEInstallDir = (Resolve-Path -LiteralPath $Candidate).Path
        break
    }
}

if (-not $UEInstallDir) {
    Write-Error @"
Unreal Engine $EngineAssociation not found.
Use -EnginePath to specify a matching installation,
or install UE $EngineAssociation via the Epic Games Launcher.
"@
    exit 1
}

Write-Host "Engine Path: $UEInstallDir" -ForegroundColor Green
Write-Host ""

# --- Target name follows UE convention: <ProjectName><Target> ---
$TargetNameMap = @{
    "Game"     = $ProjectName
    "Editor"   = "$ProjectName`Editor"
    "Client"   = "$ProjectName`Client"
    "Server"   = "$ProjectName`Server"
}
$FullTarget = $TargetNameMap[$Target]

# --- Build.bat path ---
$BuildBat = "$UEInstallDir\Engine\Build\BatchFiles\Build.bat"
if (-not (Test-Path $BuildBat)) {
    Write-Error "Build.bat not found at $BuildBat"
    exit 1
}

# --- Clean ---
if ($Clean) {
    Write-Host "Cleaning build artifacts..." -ForegroundColor Yellow
    $CleanDirs = @(
        "$ProjectRoot\Intermediate",
        "$ProjectRoot\Binaries",
        "$ProjectRoot\Saved\Logs"
    )
    foreach ($dir in $CleanDirs) {
        if (Test-Path $dir) {
            Write-Host "  Removing: $dir" -ForegroundColor DarkGray
            Remove-Item -Recurse -Force $dir -ErrorAction SilentlyContinue
        }
    }
    Write-Host "Clean complete." -ForegroundColor Green
    Write-Host ""
}

# --- Build ---
# Build.bat <TargetName> <Platform> <Configuration> <ProjectPath> -waitmutex
$BuildArgs = @(
    $FullTarget,
    $Platform,
    $Configuration,
    "`"$($ProjectFile.FullName)`"",
    "-waitmutex"
)

Write-Host "Building $FullTarget ($Configuration | $Platform)..." -ForegroundColor Yellow
Write-Host ""

& cmd.exe /c "`"$BuildBat`" $($BuildArgs -join ' ')"

$ExitCode = $LASTEXITCODE

Write-Host ""
if ($ExitCode -eq 0) {
    Write-Host "========================================" -ForegroundColor Green
    Write-Host " BUILD SUCCEEDED" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green

    if ($Run) {
        Write-Host ""
        Write-Host "Launching..." -ForegroundColor Yellow

        if ($Target -eq "Editor") {
            $EditorExe = "$UEInstallDir\Engine\Binaries\Win64\UnrealEditor.exe"
            if (-not (Test-Path $EditorExe)) {
                Write-Error "UnrealEditor.exe not found at $EditorExe"
                exit 1
            }
            Write-Host "  $EditorExe" -ForegroundColor DarkGray
            Write-Host "  $($ProjectFile.FullName)" -ForegroundColor DarkGray
            & $EditorExe "`"$($ProjectFile.FullName)`""
        }
        else {
            $GameExe = "$ProjectRoot\Binaries\$Platform\$ProjectName.exe"
            if ($Target -eq "Client") {
                $GameExe = "$ProjectRoot\Binaries\$Platform\$ProjectName`Client.exe"
            }
            elseif ($Target -eq "Server") {
                $GameExe = "$ProjectRoot\Binaries\$Platform\$ProjectName`Server.exe"
            }
            if (-not (Test-Path $GameExe)) {
                Write-Error "Executable not found: $GameExe"
                exit 1
            }
            Write-Host "  $GameExe" -ForegroundColor DarkGray
            & $GameExe
        }
    }
}
else {
    Write-Host "========================================" -ForegroundColor Red
    Write-Host " BUILD FAILED (Exit code: $ExitCode)" -ForegroundColor Red
    Write-Host "========================================" -ForegroundColor Red
}

exit $ExitCode
