#include "UnLuaGameplayTagExporter.h"

#include "UnLuaEx.h"
#include "GameplayTagContainer.h"

// clang-format off

// 导出 FGameplayTag
BEGIN_EXPORT_REFLECTED_CLASS(FGameplayTag)
    ADD_FUNCTION(IsValid)
    ADD_FUNCTION(ToString)
    ADD_STATIC_FUNCTION(RequestGameplayTag)
END_EXPORT_CLASS()
IMPLEMENT_EXPORTED_CLASS(FGameplayTag)

// 导出 FGameplayTagContainer
BEGIN_EXPORT_REFLECTED_CLASS(FGameplayTagContainer)
    ADD_FUNCTION(HasTag)
    ADD_FUNCTION(AddTag)
    ADD_FUNCTION(RemoveTag)
    ADD_FUNCTION(IsEmpty)
END_EXPORT_CLASS()
IMPLEMENT_EXPORTED_CLASS(FGameplayTagContainer)

// clang-format on