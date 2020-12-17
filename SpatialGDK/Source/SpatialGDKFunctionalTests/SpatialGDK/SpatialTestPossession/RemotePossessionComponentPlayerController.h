// Copyright (c) Improbable Worlds Ltd, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "RemotePossessionComponentPlayerController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogRemotePossessionComponentPlayerController, Log, All);


UCLASS()
class ARemotePossessionComponentPlayerController : public APlayerController
{
	GENERATED_BODY()
private:
	virtual void OnPossess(APawn* InPawn) override;

public:
	ARemotePossessionComponentPlayerController();

	UFUNCTION(Server, Reliable)
	void RemotePossess(APawn* InPawn);

	static void ResetCalledCounter();

	static int32 OnPossessCalled;
};
