// Copyright (c) Improbable Worlds Ltd, All Rights Reserved

#include "RemotePossessionComponentPlayerController.h"
#include "Engine/World.h"
#include "EngineClasses/SpatialPossession.h"
#include "Utils/SpatialStatics.h"
#include "EngineClasses/Components/RemotePossessionComponent.h"

DEFINE_LOG_CATEGORY(LogRemotePossessionComponentPlayerController);

int32 ARemotePossessionComponentPlayerController::OnPossessCalled = 0;

ARemotePossessionComponentPlayerController::ARemotePossessionComponentPlayerController()
{

}

void ARemotePossessionComponentPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	++OnPossessCalled;
	UE_LOG(LogRemotePossessionComponentPlayerController, Log, TEXT("%s OnPossess(%s) OnPossessCalled:%d"), *GetName(), *InPawn->GetName(),
		   OnPossessCalled);
}

void ARemotePossessionComponentPlayerController::RemotePossess_Implementation(APawn* InPawn)
{
	UE_LOG(LogRemotePossessionComponentPlayerController, Log, TEXT("%s RemotePossess_Implementation:%s"), *GetName(), *InPawn->GetName());
	URemotePossessionComponent* NewComponent =
		NewObject<URemotePossessionComponent>(this, URemotePossessionComponent::StaticClass(), NAME_None);
	NewComponent->Target = InPawn;
	NewComponent->RegisterComponent();
}

void ARemotePossessionComponentPlayerController::ResetCalledCounter()
{
	OnPossessCalled = 0;
}
