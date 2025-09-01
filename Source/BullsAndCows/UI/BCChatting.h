// BCChatting.h

#pragma once

#include "Blueprint/UserWidget.h"
#include "BCChatting.generated.h"

class UEditableTextBox;
/**
 * 
 */
UCLASS()
class BULLSANDCOWS_API UBCChatting : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
	void OnChatMessageCommitted(const FText& Text, ETextCommit::Type CommitInfo);
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox_Chat;
};
