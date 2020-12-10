// Copyright (c) Improbable Worlds Ltd, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "SpatialFunctionalTest.h"

#include "CrossServerPossessionTest.generated.h"

UCLASS()
class SPATIALGDKFUNCTIONALTESTS_API ACrossServerPossessionTest : public ASpatialFunctionalTest
{
	GENERATED_BODY()

public:
	ACrossServerPossessionTest();

	virtual void PrepareTest() override;

private:
	APawn* GetPawn();
	void CreateController(int Index, FVector Position);
	void CheckControllerHasAuthority(int Index);
	void RemotePossess(int Index);

	UFUNCTION()
	void OnPossess(APawn* Pawn, APlayerController* Controller);

	UFUNCTION()
	void OnUnPossess(APlayerController* Controller);

	float WaitTime;
	const static float MaxWaitTime;
};
