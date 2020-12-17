// Copyright (c) Improbable Worlds Ltd, All Rights Reserved

#include "TestPossessionPlayerController.h"
#include "Engine/World.h"
#include "EngineClasses/SpatialPossession.h"
#include "Utils/SpatialStatics.h"
#include "EngineClasses/Components/RemotePossessionComponent.h"

DEFINE_LOG_CATEGORY(LogTestPossessionPlayerController);

int32 ATestPossessionPlayerController::OnPossessCalled = 0;
int32 ATestPossessionPlayerController::OnPossessFailedCalled = 0;

ATestPossessionPlayerController::ATestPossessionPlayerController()
{

}

void ATestPossessionPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	++OnPossessCalled;
	UE_LOG(LogTestPossessionPlayerController, Log, TEXT("%s OnPossess(%s) OnPossessCalled:%d"), *GetName(), *InPawn->GetName(),
		   OnPossessCalled);
}

void ATestPossessionPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	UE_LOG(LogTestPossessionPlayerController, Log, TEXT("%s OnUnPossess()"), *GetName());
}

void ATestPossessionPlayerController::OnPossessFailed(ERemotePossessFailure FailureReason)
{
	Super::OnPossessFailed(FailureReason);
	++OnPossessFailedCalled;
	UE_LOG(LogTestPossessionPlayerController, Log, TEXT("%s OnPossessFailed(%d) OnPossessFailedCalled:%d"), *GetName(), FailureReason,
		   OnPossessFailedCalled);
}

void ATestPossessionPlayerController::RemotePossess_Implementation(APawn* InPawn)
{
	//USpatialPossession::RemotePossess(this, InPawn);
	UE_LOG(LogTestPossessionPlayerController, Log, TEXT("%s RemotePossess_Implementation:%s"), *GetName(), *InPawn->GetName());
	URemotePossessionComponent* NewComponent =
		NewObject<URemotePossessionComponent>(this, URemotePossessionComponent::StaticClass(), NAME_None);
	NewComponent->Target = InPawn;
	NewComponent->RegisterComponent();
	//AddOwnedComponent(NewComponent);
}

void ATestPossessionPlayerController::ResetCalledCounter()
{
	OnPossessCalled = 0;
	OnPossessFailedCalled = 0;
}
