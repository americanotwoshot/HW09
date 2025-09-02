// BCPlayerController.cpp

#include "BCPlayerController.h"

#include "BullsAndCows.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameModeBase.h"
#include "GameMode/BCGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UI/BCChatting.h"
#include "UI/BCNotification.h"

ABCPlayerController::ABCPlayerController()
{
	bReplicates = true;
}

void ABCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController())
	{
		return;
	}

	if (IsValid(ChattingWidgetClass))
	{
		ChattingWidgetInstance = CreateWidget<UBCChatting>(this, ChattingWidgetClass);
		if (IsValid(ChattingWidgetInstance))
		{
			ChattingWidgetInstance->AddToViewport();
		}
	}

	if (IsValid(NotificationWidgetClass))
	{
		NotificationWidgetInstance = CreateWidget<UBCNotification>(this, NotificationWidgetClass);
		if (IsValid(NotificationWidgetInstance))
		{
			NotificationWidgetInstance->AddToViewport();
		}
	}

	FInputModeUIOnly UIMode;
	SetInputMode(UIMode);
	SetShowMouseCursor(true);
}

void ABCPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
}

void ABCPlayerController::SetChatMessage(const FString& InChatMessage)
{
	ChatMessage = InChatMessage;

	if (IsLocalController())
	{
		ServerRPCPrintChatMessage(ChatMessage);
	}
}

void ABCPlayerController::PrintChatMessage(const FString& InChatMessage)
{
	UtilFunctionLibrary::MyPrintString(this, InChatMessage, 5.f);
}

void ABCPlayerController::OnRep_NotificationText()
{
	if (NotificationWidgetInstance)
	{
		NotificationWidgetInstance->SetNotificationText(NotificationText);
	}
}

void ABCPlayerController::ServerRPCPrintChatMessage_Implementation(const FString& InChatMessage)
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM))
	{
		ABCGameModeBase* BCGM = Cast<ABCGameModeBase>(GM);
		if (IsValid(BCGM))
		{
			BCGM->PrintChatMessage(this, InChatMessage);
		}
	}
}

void ABCPlayerController::ClientRPCPrintChatMessage_Implementation(const FString& InChatMessage)
{
	PrintChatMessage(InChatMessage);
}
