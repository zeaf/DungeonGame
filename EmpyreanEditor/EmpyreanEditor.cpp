// Fill out your copyright notice in the Description page of Project Settings.

#include "EmpyreanEditor.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_GAME_MODULE(FEmpyreanEditorModule, EmpyreanEditor);

#define LOCTEXT_NAMESPACE "FEmpyreanEditorModule"

void FEmpyreanEditorModule::StartupModule()
{
	UE_LOG(LogTemp, Warning, TEXT("StartupModule() called"));
}

void FEmpyreanEditorModule::ShutdownModule()
{
	UE_LOG(LogTemp, Warning, TEXT("ShutdownModule() called"));
}

#undef LOCTEXT_NAMESPACE