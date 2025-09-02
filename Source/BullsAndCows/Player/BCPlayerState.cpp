// BCPlayerState.cpp

#include "BCPlayerState.h"

#include "Net/UnrealNetwork.h"

ABCPlayerState::ABCPlayerState()
	: PlayerName(TEXT("None"))
	, CurrentGuessCount(0)
	, MaxGuessCount(3)
	, bIsTurn(0)
{
	bReplicates = true;
}

void ABCPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerName);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	DOREPLIFETIME(ThisClass, MaxGuessCount);
	DOREPLIFETIME(ThisClass, bIsTurn);
}

FString ABCPlayerState::GetPlayerInfo()
{
	FString PlayerInfo = PlayerName + TEXT("(") + FString::FromInt(CurrentGuessCount) + TEXT("/") + FString::FromInt(MaxGuessCount) + TEXT(")");
	return PlayerInfo;
}
