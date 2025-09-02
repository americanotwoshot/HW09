// BCNotification.cpp

#include "BCNotification.h"

#include "Components/TextBlock.h"

void UBCNotification::SetNotificationText(const FText& InText)
{
	TextBlock_Notification->SetText(InText);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		TextBlock_Notification->SetText(FText());
	}), 5.f, false);
}
