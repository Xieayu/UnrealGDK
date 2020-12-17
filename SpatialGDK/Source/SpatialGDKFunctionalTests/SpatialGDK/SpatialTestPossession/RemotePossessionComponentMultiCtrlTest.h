// Copyright (c) Improbable Worlds Ltd, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "SpatialFunctionalTest.h"
#include "RemotePossessionComponentMultiCtrlTest.generated.h"

class ATestPossessionPawn;

UCLASS()
class SPATIALGDKFUNCTIONALTESTS_API ARemotePossessionComponentMultiCtrlTest : public ASpatialFunctionalTest
{
	GENERATED_BODY()

public:
	ARemotePossessionComponentMultiCtrlTest();

	virtual void PrepareTest() override;

private:
	ATestPossessionPawn* GetPawn();

	void CreateController(int Index, FVector Position);

	void CheckControllerHasAuthority(int Index);

	void AddWaitStep(const FWorkerDefinition& Worker);

	float WaitTime;
	const static float MaxWaitTime;
};
