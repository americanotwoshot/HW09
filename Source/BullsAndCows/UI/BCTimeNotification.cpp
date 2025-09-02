// BCTimeNotification.cpp

#include "BCTimeNotification.h"

#include "Components/TextBlock.h"

void UBCTimeNotification::UpdateRemainingTime(float InRemainingTime)
{
	if (TextBlock_Time)
	{
		const int32 RemainingTime = FMath::CeilToInt32(InRemainingTime);

		if (RemainingTime <= 0)
		{
			TextBlock_Time->SetText(FText());
		}
		else
		{
			TextBlock_Time->SetText(FText::FromString(FString::FromInt(RemainingTime)));
		}
	}
}
