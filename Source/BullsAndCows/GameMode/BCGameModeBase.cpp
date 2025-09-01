// BCGameModeBase.cpp

#include "BCGameModeBase.h"

#include "GameState/BCGameStateBase.h"

void ABCGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ABCGameStateBase* BCGameStateBase = GetGameState<ABCGameStateBase>();
	if (IsValid(BCGameStateBase))
	{
		BCGameStateBase->MulticastRPCBroadcastLogin();
	}
}
