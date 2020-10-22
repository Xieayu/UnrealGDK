// Copyright (c) Improbable Worlds Ltd, All Rights Reserved

#pragma once

#include "Interop/Connection/SpatialTraceEvent.h"
#include "Interop/Connection/UserSpanId.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include <WorkerSDK/improbable/c_trace.h>

#include "SpatialEventTracerUserInterface.generated.h"

DECLARE_DYNAMIC_DELEGATE(FEventTracerRPCDelegate);

DECLARE_LOG_CATEGORY_EXTERN(LogSpatialEventTracerUserInterface, Log, All);

namespace SpatialGDK
{
class SpatialEventTracer;
}

class USpatialNetDriver;

// Docs on how to use the interface can be found:
// https://docs.google.com/document/d/1i0fOdeldqeZ9kgBdmYcTD3fCwYXT9pX1RzpTIupgjcg/edit?usp=sharing

UCLASS()
class SPATIALGDK_API USpatialEventTracerUserInterface : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Creates a SpanId to be used to trace a root event.
	 * (This API is subject to change)
	 */
	UFUNCTION(BlueprintCallable, Category = "SpatialOS|EventTracing", meta = (WorldContext = "WorldContextObject"))
	static FUserSpanId CreateSpanId(UObject* WorldContextObject);

	/**
	 * Creates a SpanId to be used to trace an event with a set of causes.
	 * (This API is subject to change)
	 */
	UFUNCTION(BlueprintCallable, Category = "SpatialOS|EventTracing", meta = (WorldContext = "WorldContextObject"))
	static FUserSpanId CreateSpanIdWithCauses(UObject* WorldContextObject, const TArray<FUserSpanId>& Causes);

	/**
	 * Will trace an event using the input data and associate it with the input SpanId
	 * (This API is subject to change)
	 */
	UFUNCTION(BlueprintCallable, Category = "SpatialOS|EventTracing", meta = (WorldContext = "WorldContextObject"))
	static void TraceEvent(UObject* WorldContextObject, const FUserSpanId& UserSpanId, FSpatialTraceEvent SpatialTraceEvent);

	/**
	 * Will ensure that the input SpanId is used to continue the tracing of the RPC flow.
	 * Use the Delegate to call your RPC
	 * (This API is subject to change)
	 */
	UFUNCTION(BlueprintCallable, Category = "SpatialOS|EventTracing", meta = (WorldContext = "WorldContextObject"))
	static void TraceRPC(UObject* WorldContextObject, FEventTracerRPCDelegate Delegate, const FUserSpanId& SpanId);

	/**
	 * Will ensure that the input SpanId is used to continue the tracing of the property update flow.
	 * The input Object should be the object that contains the property
	 * (This API is subject to change)
	 */
	UFUNCTION(BlueprintCallable, Category = "SpatialOS|EventTracing", meta = (WorldContext = "WorldContextObject"))
	static void TraceProperty(UObject* WorldContextObject, UObject* Object, const FUserSpanId& UserSpanId);

	/**
	 * Used to get the active SpanId from the GDK. Use this to cause your own trace events.
	 * (This API is subject to change)
	 */
	UFUNCTION(BlueprintCallable, Category = "SpatialOS|EventTracing", meta = (WorldContext = "WorldContextObject"))
	static bool GetActiveSpanId(UObject* WorldContextObject, FUserSpanId& OutUserSpanId);

private:
	static void AddLatentActorSpanId(UObject* WorldContextObject, const AActor& Actor, const FUserSpanId& UserSpanId);
	static void AddLatentComponentSpanId(UObject* WorldContextObject, const UActorComponent& Component, const FUserSpanId& UserSpanId);

	static SpatialGDK::SpatialEventTracer* GetEventTracer(UObject* WorldContextObject);
	static USpatialNetDriver* GetSpatialNetDriver(UObject* WorldContextObject);
};
