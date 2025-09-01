// BCGameStateBase.h

#pragma once

#include "GameFramework/GameStateBase.h"
#include "BCGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class BULLSANDCOWS_API ABCGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLogin(const FString& InNameString = FString(TEXT("XXXXXX")));
};
