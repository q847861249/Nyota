#pragma once

#include "Logging/LogMacros.h"

class UObject;

NYOTA_API DECLARE_LOG_CATEGORY_EXTERN(LogNyota, Log, All);
NYOTA_API DECLARE_LOG_CATEGORY_EXTERN(LogNyotaExperience, Log, All);
NYOTA_API DECLARE_LOG_CATEGORY_EXTERN(LogNyotaAbilitySystem, Log, All);
NYOTA_API DECLARE_LOG_CATEGORY_EXTERN(LogNyotaTeams, Log, All);

NYOTA_API FString GetClientServerContextString(UObject *ContextObject = nullptr);
