// Copyright (c) Improbable Worlds Ltd, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include <WorkerSDK/improbable/c_worker.h>

namespace SpatialGDK
{
struct Component
{
	virtual ~Component() {}
	virtual void ApplyComponentUpdate(const Worker_ComponentUpdate& Update) {}
};

struct DataComponent : Component
{
	virtual struct Worker_ComponentData CreateComponentData()
	{
		checkNoEntry();
		return Worker_ComponentData{};
	}
};

} // namespace SpatialGDK
