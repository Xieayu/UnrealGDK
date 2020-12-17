// Copyright (c) Improbable Worlds Ltd, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RemotePossessionComponentGameMode.generated.h"

UCLASS()
class ARemotePossessionComponentGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ARemotePossessionComponentGameMode();
	AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName) override;

private:
	void Generate_SpawnPoints();

	int32 PlayersSpawned;
	bool bInitializedSpawnPoints;
	TArray<AActor*> SpawnPoints;
	TMap<int32, AActor*> PlayerIdToSpawnPointMap;
};
