// Copyright Epic Games, Inc. All Rights Reserved.

#include "K_CardsRuntimeModule.h"

#include "GameplayTagsManager.h"

#define LOCTEXT_NAMESPACE "FK_CardsRuntimeModule"

void FK_CardsRuntimeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory;
	// the exact timing is specified in the .uplugin file per-module

	// Ensure Gameplay Tags are loaded
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();
	TagsManager.LoadGameplayTagTables();
}

void FK_CardsRuntimeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.
	// For modules that support dynamic reloading, we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FK_CardsRuntimeModule, K_CardsRuntime)
