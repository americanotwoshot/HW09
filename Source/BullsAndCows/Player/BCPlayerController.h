// BCPlayerController.h

#pragma once

#include "GameFramework/PlayerController.h"
#include "BCPlayerController.generated.h"

class UBCNotification;
class UBCChatting;
/**
 * 
 */
UCLASS()
class BULLSANDCOWS_API ABCPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABCPlayerController();

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	void SetChatMessage(const FString& InChatMessage);
	void PrintChatMessage(const FString& InChatMessage);

	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessage(const FString& InChatMessage);

	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessage(const FString& InChatMessage);

	UFUNCTION()
	void OnRep_NotificationText();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "BCPlayerController|Widget")
	TSubclassOf<UBCChatting> ChattingWidgetClass;
	UPROPERTY()
	TObjectPtr<UBCChatting> ChattingWidgetInstance;

	FString ChatMessage;

	UPROPERTY(EditDefaultsOnly, Category = "BCPlayerController|Widget")
	TSubclassOf<UBCNotification> NotificationWidgetClass;
	UPROPERTY()
	TObjectPtr<UBCNotification> NotificationWidgetInstance;

public:
	UPROPERTY(ReplicatedUsing = OnRep_NotificationText)
	FText NotificationText;
};
