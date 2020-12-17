// Copyright (c) Improbable Worlds Ltd, All Rights Reserved

#include "RemotePossessionComponentGameMode.h"

#include "GameFramework/PlayerStart.h"
#include "RemotePossessionComponentPlayerController.h"

ARemotePossessionComponentGameMode::ARemotePossessionComponentGameMode()
	: PlayersSpawned(0)
	, bInitializedSpawnPoints(false)
{
	PlayerControllerClass = ARemotePossessionComponentPlayerController::StaticClass();
}

AActor* ARemotePossessionComponentGameMode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
	Generate_SpawnPoints();

	if (Player == nullptr)
	{
		return SpawnPoints[PlayersSpawned % SpawnPoints.Num()];
	}

	const int32 PlayerUniqueID = Player->GetUniqueID();
	AActor** SpawnPoint = PlayerIdToSpawnPointMap.Find(PlayerUniqueID);
	if (SpawnPoint != nullptr)
	{
		return *SpawnPoint;
	}

	AActor* ChosenSpawnPoint = SpawnPoints[PlayersSpawned % SpawnPoints.Num()];
	PlayerIdToSpawnPointMap.Add(PlayerUniqueID, ChosenSpawnPoint);

	PlayersSpawned++;

	return ChosenSpawnPoint;
}

void ARemotePossessionComponentGameMode::Generate_SpawnPoints()
{
	if (false == bInitializedSpawnPoints)
	{
		UWorld* World = GetWorld();

		FActorSpawnParameters SpawnInfo{};
		SpawnInfo.Owner = this;
		SpawnInfo.Instigator = NULL;
		SpawnInfo.bDeferConstruction = false;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		SpawnPoints.Add(World->SpawnActor<APlayerStart>(APlayerStart::StaticClass(), FVector(-500.0f, -500.0f, 50.0f),
														FRotator::ZeroRotator, SpawnInfo));
		SpawnPoints.Add(World->SpawnActor<APlayerStart>(APlayerStart::StaticClass(), FVector(500.0f, -500.0f, 50.0f), FRotator::ZeroRotator,
														SpawnInfo));
		SpawnPoints.Add(World->SpawnActor<APlayerStart>(APlayerStart::StaticClass(), FVector(-500.0f, 500.0f, 50.0f), FRotator::ZeroRotator,
														SpawnInfo));

		bInitializedSpawnPoints = true;
	}
}
