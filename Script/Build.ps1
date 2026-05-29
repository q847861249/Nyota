param(
    [ValidateSet("Game", "Editor", "Client", "Server")]
    [string]$Target = "Game",

    [ValidateSet("Development", "Debug", "Shipping", "DebugGame", "Test")]
    [string]$Configuration = "Development",

    [ValidateSet("Win64", "Linux", "Mac")]
    [string]$Platform = "Win64",

    [switch]$Clean,
    [switch]$Run
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

# --- Find Unreal Engine ---
$UERegPath = "HKLM:\SOFTWARE\EpicGames\Unreal Engine\5.4"
$UERegPathWOW = "HKLM:\SOFTWARE\WOW6432Node\EpicGames\Unreal Engine\5.4"
$UEInstallDir = $null

try { $UEInstallDir = (Get-ItemProperty -Path $UERegPath -ErrorAction Stop).InstalledDirectory }
catch {
    try { $UEInstallDir = (Get-ItemProperty -Path $UERegPathWOW -ErrorAction Stop).InstalledDirectory }
    catch { }
}

if (-not $UEInstallDir) {
    $CustomPaths = @(
        "D:\UE\UE_5.4",
        "C:\UE\UE_5.4",
        "C:\Program Files\Epic Games\UE_5.4",
        "D:\Program Files\Epic Games\UE_5.4"
    )
    foreach ($p in $CustomPaths) {
        if (Test-Path "$p\Engine\Binaries\Win64\UnrealBuildTool.exe") {
            $UEInstallDir = $p
            break
        }
    }
}

if (-not $UEInstallDir) {
    Write-Error @"
Unreal Engine 5.4 not found.
Please set the engine path manually by editing this script,
or install UE 5.4 via the Epic Games Launcher.
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
