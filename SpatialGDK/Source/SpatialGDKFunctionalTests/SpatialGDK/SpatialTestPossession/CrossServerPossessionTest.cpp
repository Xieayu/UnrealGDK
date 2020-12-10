// Copyright (c) Improbable Worlds Ltd, All Rights Reserved

#include "CrossServerPossessionTest.h"

#include "Containers/Array.h"
#include "EngineClasses/SpatialNetDriver.h"
#include "EngineClasses/SpatialPossession.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerController.h"
#include "GameMapsSettings.h"
#include "Kismet/GameplayStatics.h"
#include "LoadBalancing/LayeredLBStrategy.h"
#include "SpatialFunctionalTestFlowController.h"
#include "SpatialGDKFunctionalTests/SpatialGDK/TestActors/TestMovementCharacter.h"
#include "TestPossessionPawn.h"
#include "TestPossessionPlayerController.h"
#include "Utils/SpatialStatics.h"

const float ACrossServerPossessionTest::MaxWaitTime = 1.0f;

ACrossServerPossessionTest::ACrossServerPossessionTest()
	: Super()
	, WaitTime(0.0f)
{
	Author = "Ken.Yu";
	Description = TEXT("Test Cross-Server Possession");
}

void ACrossServerPossessionTest::PrepareTest()
{
	Super::PrepareTest();

	AddStep(TEXT("EnsureSpatialOS"), FWorkerDefinition::AllServers, nullptr, [this]() {
		ULayeredLBStrategy* LoadBalanceStrategy = GetLoadBalancingStrategy();
		if (LoadBalanceStrategy == nullptr)
		{
			FinishTest(EFunctionalTestResult::Error, TEXT("Test requires SpatialOS enabled with Load-Balancing Strategy"));
		}
		else
		{
			if (AGameModeBase* GameMode = Cast<AGameModeBase>(AGameModeBase::StaticClass()->GetDefaultObject()))
			{
				LogStep(ELogVerbosity::Log, FString::Printf(TEXT("GameMode:%s"), *GameMode->GetFullName()));
			}
			FinishStep();
		}
	});

	AddStep(TEXT("Cross-Server Possession: Create Pawn"), FWorkerDefinition::Server(1), nullptr, [this]() {
		// Place the Pawn in worker 1
		GetWorld()->SpawnActor<ATestPossessionPawn>(FVector(500.0f, 0.0f, 50.0f), FRotator::ZeroRotator, FActorSpawnParameters());
		FinishStep();
	});

	AddStep(TEXT("Cross-Server Possession: Create Controller"), FWorkerDefinition::Server(2), nullptr, nullptr, [this](float DeltaTime) {
		// Place the Controller in worker 2
		CreateController(1, FVector(-500.0f, 0.0f, 50.0f));
		FinishStep();
	});

	// Shown the Controller and Pawn in which worker
	AddStep(TEXT("Cross-Server Possession: Shown who should have authority"), FWorkerDefinition::Server(1), nullptr, nullptr,
			[this](float DeltaTime) {
				ULayeredLBStrategy* LoadBalanceStrategy = GetLoadBalancingStrategy();
				if (LoadBalanceStrategy != nullptr)
				{
					if (APawn* Pawn = GetPawn())
					{
						for (ASpatialFunctionalTestFlowController* FlowController : GetFlowControllers())
						{
							AController* Controller = Cast<AController>(FlowController->GetOwner());
							if (Controller != nullptr)
							{
								uint32 WorkerId = LoadBalanceStrategy->WhoShouldHaveAuthority(*Controller);
								LogStep(ELogVerbosity::Log, FString::Printf(TEXT("Controller:%s authoritatived in worker: %d"),
																			*Controller->GetFullName(), WorkerId));
							}
						}

						uint32 WorkerId = LoadBalanceStrategy->WhoShouldHaveAuthority(*Pawn);
						LogStep(ELogVerbosity::Log,
								FString::Printf(TEXT("Pawn:%s authoritatived in worker: %d"), *GetPawn()->GetFullName(), WorkerId));
					}
					else
					{
						FinishTest(EFunctionalTestResult::Error, TEXT("Couldn't found pawn for possession"));
					}
				}
				else
				{
					FinishTest(EFunctionalTestResult::Error, TEXT("Test requires SpatialOS enabled with Load-Balancing Strategy"));
				}
				FinishStep();
			});

	// Call RemotePossess
	AddStep(TEXT("Cross-Server Possession: controller remote possess"), FWorkerDefinition::Server(2), nullptr, [this]() {
		RemotePossess(1);
		FinishStep();
	});

	// Wait 1 seconds to make sure it happend
	AddStep(TEXT("Cross-Server Possession: Wait"), FWorkerDefinition::Server(1), nullptr, nullptr, [this](float DeltaTime) {
		if (WaitTime > MaxWaitTime)
		{
			FinishStep();
		}
		WaitTime += DeltaTime;
	});

	// Check RemotePossess result
	AddStep(TEXT("Cross-Server Possession: Check results"), FWorkerDefinition::Server(4), nullptr, nullptr, [this](float DeltaTime) {
		if (APawn* Pawn = GetPawn())
		{
			if (AController* Controller = Pawn->GetController())
			{
				FinishTest(EFunctionalTestResult::Succeeded, FString::Printf(TEXT("%s possessed the Pawn"), *Controller->GetFullName()));
			}
			else
			{
				FinishTest(EFunctionalTestResult::Error, TEXT("Possess failed!"));
			}
		}
		else
		{
			FinishTest(EFunctionalTestResult::Error, TEXT("Couldn't found pawn for possession"));
		}
	});
}

APawn* ACrossServerPossessionTest::GetPawn()
{
	return Cast<APawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ATestPossessionPawn::StaticClass()));
}

void ACrossServerPossessionTest::CreateController(int Index, FVector Position)
{
	ASpatialFunctionalTestFlowController* FlowController = GetFlowController(ESpatialFunctionalTestWorkerType::Client, Index);
	ATestPossessionPlayerController* PlayerController = Cast<ATestPossessionPlayerController>(FlowController->GetOwner());
	if (IsValid(PlayerController))
	{
		ATestMovementCharacter* Character =
			GetWorld()->SpawnActor<ATestMovementCharacter>(Position, FRotator::ZeroRotator, FActorSpawnParameters());
		RegisterAutoDestroyActor(Character);
		USpatialPossession::RemotePossess(PlayerController, Character);
	}
	else
	{
		FinishTest(EFunctionalTestResult::Error, TEXT("Failed to get PlayerController"));
	}
}

void ACrossServerPossessionTest::CheckControllerHasAuthority(int Index)
{
	ASpatialFunctionalTestFlowController* FlowController = GetFlowController(ESpatialFunctionalTestWorkerType::Client, Index);
	ATestPossessionPlayerController* PlayerController = Cast<ATestPossessionPlayerController>(FlowController->GetOwner());
	if (IsValid(PlayerController))
	{
		AssertTrue(PlayerController->HasAuthority(), TEXT("PlayerController HasAuthority"), PlayerController);
	}
	else
	{
		FinishTest(EFunctionalTestResult::Error, TEXT("Failed to get PlayerController"));
	}
}

void ACrossServerPossessionTest::RemotePossess(int Index)
{
	if (APawn* Pawn = GetPawn())
	{
		ASpatialFunctionalTestFlowController* FlowController = GetFlowController(ESpatialFunctionalTestWorkerType::Client, Index);
		ATestPossessionPlayerController* Controller = Cast<ATestPossessionPlayerController>(FlowController->GetOwner());
		if (IsValid(Controller))
		{
			if (Controller->HasAuthority())
			{
				LogStep(ELogVerbosity::Log,
						FString::Printf(TEXT("%s try to remote possess %s"), *Controller->GetFullName(), *Pawn->GetFullName()));
				Controller->OnPossessEvent.AddDynamic(this, &ACrossServerPossessionTest::OnPossess);
				Controller->OnUnPossessEvent.AddDynamic(this, &ACrossServerPossessionTest::OnUnPossess);
				USpatialPossession::RemotePossess(Controller, Pawn);
			}
		}
		else
		{
			FinishTest(EFunctionalTestResult::Error, TEXT("Failed to get PlayerController"));
		}
	}
	else
	{
		FinishTest(EFunctionalTestResult::Error, TEXT("Failed to get the pawn for possession"));
	}
}

void ACrossServerPossessionTest::OnPossess(APawn* Pawn, APlayerController* Controller)
{
	LogStep(ELogVerbosity::Log, FString::Printf(TEXT("OnPossess Pawn:%s"), *Pawn->GetFullName()));
}

void ACrossServerPossessionTest::OnUnPossess(APlayerController* Controller)
{
	LogStep(ELogVerbosity::Log, TEXT("OnUnPossess"));
}
