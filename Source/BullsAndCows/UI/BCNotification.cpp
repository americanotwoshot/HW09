// BCNotification.cpp

#include "BCNotification.h"

#include "Components/TextBlock.h"

void UBCNotification::SetNotificationText(const FString& InText)
{
	TextBlock_Notification->SetText(FText::FromString(InText));

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		TextBlock_Notification->SetText(FText::GetEmpty());
	}), 5.f, false);
}
