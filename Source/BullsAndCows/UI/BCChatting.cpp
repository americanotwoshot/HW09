// BCChatting.cpp

#include "BCChatting.h"

#include "Components/EditableTextBox.h"
#include "Player/BCPlayerController.h"

void UBCChatting::NativeConstruct()
{
	Super::NativeConstruct();

	if (!EditableTextBox_Chat->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatMessageCommitted))
	{
		EditableTextBox_Chat->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatMessageCommitted);
	}
}

void UBCChatting::NativeDestruct()
{
	Super::NativeDestruct();

	if (EditableTextBox_Chat->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatMessageCommitted))
	{
		EditableTextBox_Chat->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatMessageCommitted);
	}
}

void UBCChatting::OnChatMessageCommitted(const FText& Text, ETextCommit::Type CommitInfo)
{
	if (CommitInfo == ETextCommit::OnEnter)
	{
		APlayerController* OwningPC = GetOwningPlayer();
		if (IsValid(OwningPC))
		{
			ABCPlayerController* OwningBCPC = Cast<ABCPlayerController>(OwningPC);
			if (IsValid(OwningBCPC))
			{
				OwningBCPC->SetChatMessage(Text.ToString());

				EditableTextBox_Chat->SetText(FText());
			}
		}
	}
}
