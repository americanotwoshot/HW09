// BCNotification.h

#pragma once

#include "Blueprint/UserWidget.h"
#include "BCNotification.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class BULLSANDCOWS_API UBCNotification : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetNotificationText(const FText& InText);

private:
	FTimerHandle TimerHandle;

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Notification;
};
