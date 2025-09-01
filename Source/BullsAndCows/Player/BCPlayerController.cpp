// BCPlayerController.cpp

#include "BCPlayerController.h"

#include "BullsAndCows.h"
#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "UI/BCChatting.h"

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

void ABCPlayerController::ServerRPCPrintChatMessage_Implementation(const FString& InChatMessage)
{
	for (TActorIterator<ABCPlayerController> It(GetWorld()); It; ++It)
	{
		ABCPlayerController* BCPC = *It;
		if (IsValid(BCPC))
		{
			BCPC->ClientRPCPrintChatMessage(InChatMessage);	
		}
	}
}

void ABCPlayerController::ClientRPCPrintChatMessage_Implementation(const FString& InChatMessage)
{
	PrintChatMessage(InChatMessage);
}
