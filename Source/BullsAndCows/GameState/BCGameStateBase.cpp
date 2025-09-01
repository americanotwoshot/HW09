// BCGameStateBase.cpp

#include "BCGameStateBase.h"

#include "Kismet/GameplayStatics.h"
#include "Player/BCPlayerController.h"

void ABCGameStateBase::MulticastRPCBroadcastLogin_Implementation(const FString& InNameString)
{
	if (!HasAuthority())
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC))
		{
			ABCPlayerController* BCPC = Cast<ABCPlayerController>(PC);
			if (IsValid(BCPC))
			{
				FString NotificationString = InNameString + " has logged in.";
				BCPC->PrintChatMessage(NotificationString);
			}
		}
	}
}
