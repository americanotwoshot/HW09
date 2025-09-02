// BCPlayerState.h

#pragma once

#include "GameFramework/PlayerState.h"
#include "BCPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BULLSANDCOWS_API ABCPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ABCPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	FString GetPlayerInfo();
	
public:
	UPROPERTY(Replicated)
	FString PlayerName;

	UPROPERTY(Replicated)
	int32 CurrentGuessCount;

	UPROPERTY(Replicated)
	int32 MaxGuessCount;

	UPROPERTY(Replicated)
	uint8 bIsTurn : 1;
};
