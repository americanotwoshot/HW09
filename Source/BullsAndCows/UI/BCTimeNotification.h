// BCTimeNotification.h

#pragma once

#include "Blueprint/UserWidget.h"
#include "BCTimeNotification.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class BULLSANDCOWS_API UBCTimeNotification : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateRemainingTime(float InRemainingTime);
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Time;
};
