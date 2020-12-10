// Copyright (c) Improbable Worlds Ltd, All Rights Reserved

#include "TestPossessionPlayerController.h"
#include "Engine/World.h"

ATestPossessionPlayerController::ATestPossessionPlayerController() {}

void ATestPossessionPlayerController::OnPossess(APawn* InPawn)
{
	OnPossessEvent.Broadcast(InPawn, this);
}

void ATestPossessionPlayerController::OnUnPossess()
{
	OnUnPossessEvent.Broadcast(this);
}
