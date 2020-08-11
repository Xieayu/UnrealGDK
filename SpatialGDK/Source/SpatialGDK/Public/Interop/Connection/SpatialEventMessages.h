// Copyright (c) Improbable Worlds Ltd, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "SpatialCommonTypes.h"

#include <WorkerSDK/improbable/c_worker.h>

#include "SpatialEventMessages.generated.h"

static_assert(sizeof(Worker_EntityId) == sizeof(int64), "EntityId assumed 64-bit here");
static_assert(sizeof(Worker_ComponentId) == sizeof(uint32), "ComponentId assumed 32-bit here");
static_assert(sizeof(VirtualWorkerId) == sizeof(uint32), "VirtualWorkerId assumed 32-bit here");
static_assert(sizeof(TraceKey) == sizeof(int32), "TraceKey assumed 32-bit here");
static_assert(sizeof(Worker_RequestId) == sizeof(int64), "RequestId assumed 64-bit here");

USTRUCT()
struct FEventMessage
{
	GENERATED_BODY()

	FEventMessage() = default;
	FEventMessage(const char* InType)
		: Type(InType)
	{}

	const char* GetType() const { return Type; }

private:

	const char* Type = "Null";
};

USTRUCT()
struct FEventGenericMessage : public FEventMessage
{
	GENERATED_BODY()

	FEventGenericMessage()
		: FEventMessage("GenericMessage")
	{}
	FEventGenericMessage(const FString& Message)
		: FEventMessage("GenericMessage")
		, Message(Message)
	{}

	UPROPERTY() FString Message;
};

USTRUCT()
struct FEventCreateEntity : public FEventMessage
{
	GENERATED_BODY()

	FEventCreateEntity()
		: FEventMessage("CreateEntity")
	{}
	FEventCreateEntity(int64 EntityId, const AActor* Actor)
		: FEventMessage("CreateEntity")
		, EntityId(EntityId)
		, Actor(Actor)
	{}

	UPROPERTY() int64 EntityId = -1;
	UPROPERTY() const AActor* Actor = nullptr;
};

USTRUCT()
struct FEventRemoveEntity : public FEventMessage
{
	GENERATED_BODY()

	FEventRemoveEntity()
		: FEventMessage("RemoveEntity")
	{}
	FEventRemoveEntity(int64 EntityId, const AActor* Actor)
		: FEventMessage("RemoveEntity")
		, EntityId(EntityId)
		, Actor(Actor)
	{}

	UPROPERTY() int64 EntityId { -1 };
	UPROPERTY() const AActor* Actor { nullptr };
};

USTRUCT()
struct FEventCreateEntitySuccess : public FEventMessage
{
	GENERATED_BODY()

	FEventCreateEntitySuccess()
		: FEventMessage("CreateEntitySuccess")
	{}
	FEventCreateEntitySuccess(int64 EntityId, const AActor* Actor)
		: FEventMessage("CreateEntitySuccess")
		, EntityId(EntityId)
		, Actor(Actor)
	{}

	UPROPERTY() int64 EntityId { -1 };
	UPROPERTY() const AActor* Actor { nullptr };
};

USTRUCT()
struct FEventAuthorityIntentUpdate : public FEventMessage
{
	GENERATED_BODY()

	FEventAuthorityIntentUpdate()
		: FEventMessage("AuthorityIntentUpdate")
	{}
	FEventAuthorityIntentUpdate(uint32 NewWorkerId, const AActor* Actor)
		: FEventMessage("AuthorityIntentUpdate")
		, NewWorkerId(NewWorkerId)
		, Actor(Actor)
	{}

	UPROPERTY() uint32 NewWorkerId { 0xFFFFFFFF };
	UPROPERTY() const AActor* Actor { nullptr };
};

USTRUCT()
struct FEventAuthorityLossImminent : public FEventMessage
{
	GENERATED_BODY()

	FEventAuthorityLossImminent()
		: FEventMessage("AuthorityLossImminent")
	{}
	FEventAuthorityLossImminent(TEnumAsByte<ENetRole> Role, const AActor* Actor)
		: FEventMessage("AuthorityLossImminent")
		, Role(Role)
		, Actor(Actor)
	{}

	UPROPERTY() TEnumAsByte<ENetRole> Role{ 0 };
	UPROPERTY() const AActor* Actor { nullptr };
};

USTRUCT()
struct FEventRetireEntityRequest : public FEventMessage
{
	GENERATED_BODY()

	FEventRetireEntityRequest()
		: FEventMessage("EntityRetire")
	{}
	FEventRetireEntityRequest(int64 EntityId, const AActor* Actor)
		: FEventMessage("EntityRetire")
		, EntityId(EntityId)
		, Actor(Actor)
	{}

	UPROPERTY() int64 EntityId { -1 };
	UPROPERTY() const AActor* Actor { nullptr };
};

USTRUCT()
struct FEventSendRPC : public FEventMessage
{
	GENERATED_BODY()

	FEventSendRPC()
		: FEventMessage("SendRPC")
	{}
	FEventSendRPC(const UObject* TargetObject, const UFunction* Function)
		: FEventMessage("SendRPC")
		, TargetObject(TargetObject)
		, Function(Function)
	{}

	UPROPERTY() const UObject* TargetObject { nullptr };
	UPROPERTY() const UFunction* Function { nullptr };
};

USTRUCT()
struct FEventRPCQueued : public FEventMessage
{
	GENERATED_BODY()

	FEventRPCQueued()
		: FEventMessage("RPCQueued")
	{}
	FEventRPCQueued(const UObject* TargetObject, const UFunction* Function)
		: FEventMessage("RPCQueued")
		, TargetObject(TargetObject)
		, Function(Function)
	{}

	UPROPERTY() const UObject* TargetObject { nullptr };
	UPROPERTY() const UFunction* Function { nullptr };
};

USTRUCT()
struct FEventRPCRetried : public FEventMessage
{
	GENERATED_BODY()

	FEventRPCRetried()
		: FEventMessage("RPCRetried")
	{}
};

USTRUCT()
struct FEventRPCProcessed : public FEventMessage
{
	GENERATED_BODY()

	FEventRPCProcessed()
		: FEventMessage("RPCProcessed")
	{}
	FEventRPCProcessed(const UObject* TargetObject, const UFunction* Function)
		: FEventMessage("RPCProcessed")
		, TargetObject(TargetObject)
		, Function(Function)
	{}

	UPROPERTY() const UObject* TargetObject { nullptr };
	UPROPERTY() const UFunction* Function { nullptr };
};

USTRUCT()
struct FEventComponentUpdate : public FEventMessage
{
	GENERATED_BODY()

	FEventComponentUpdate()
		: FEventMessage("ComponentUpdate")
	{}
	FEventComponentUpdate(const AActor* Actor, const UObject* TargetObject, uint32 ComponentId)
		: FEventMessage("ComponentUpdate")
		, Actor(Actor)
		, TargetObject(TargetObject)
		, ComponentId(ComponentId)
	{}

	UPROPERTY() const AActor* Actor { nullptr };
	UPROPERTY() const UObject* TargetObject { nullptr };
	UPROPERTY() uint32 ComponentId { 0 };
};

USTRUCT()
struct FEventCommandResponse : public FEventMessage
{
	GENERATED_BODY()

	FEventCommandResponse()
		: FEventMessage("CommandResponse")
	{}
	FEventCommandResponse(const FString& Command)
		: FEventMessage("CommandResponse")
		, Command(Command)
	{}
	FEventCommandResponse(const FString& Command, int64 RequestID)
		: FEventMessage("CommandResponse")
		, Command(Command)
		, RequestID(RequestID)
	{}
	FEventCommandResponse(const FString& Command, const AActor* Actor, const UObject* TargetObject, const UFunction* Function, int64 RequestID, bool bSuccess)
		: FEventMessage("CommandResponse")
		, Command(Command)
		, Actor(Actor)
		, TargetObject(TargetObject)
		, Function(Function)
		, RequestID(RequestID)
		, bSuccess(bSuccess)
	{}

	UPROPERTY() FString Command;
	UPROPERTY() const AActor* Actor { nullptr };
	UPROPERTY() const UObject* TargetObject { nullptr };
	UPROPERTY() const UFunction* Function { nullptr };
	UPROPERTY() int64 RequestID { -1 };
	UPROPERTY() bool bSuccess{ false };
};

USTRUCT()
struct FEventCommandRequest : public FEventMessage
{
	GENERATED_BODY()

	FEventCommandRequest()
		: FEventMessage("CommandRequest")
	{}
	FEventCommandRequest(const FString& Command, int64 RequestID)
		: FEventMessage("CommandRequest")
		, Command(Command)
		, RequestID(RequestID)
	{}
	FEventCommandRequest(const FString& Command, const AActor* Actor, const UObject* TargetObject, const UFunction* Function, int32 TraceId, int64 RequestID)
		: FEventMessage("CommandRequest")
		, Command(Command)
		, Actor(Actor)
		, TargetObject(TargetObject)
		, Function(Function)
		, TraceId(TraceId)
		, RequestID(RequestID)
	{}

	UPROPERTY() FString Command;
	UPROPERTY() const AActor* Actor { nullptr };
	UPROPERTY() const UObject* TargetObject { nullptr };
	UPROPERTY() const UFunction* Function { nullptr };
	UPROPERTY() int32 TraceId { -1 };
	UPROPERTY() int64 RequestID { -1 };
};