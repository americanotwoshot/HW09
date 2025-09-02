// BCGameModeBase.cpp

#include "BCGameModeBase.h"

#include "GameState/BCGameStateBase.h"
#include "Player/BCPlayerController.h"
#include "Player/BCPlayerState.h"

void ABCGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ABCPlayerController* PC = Cast<ABCPlayerController>(NewPlayer);
	if (IsValid(PC))
	{
		PC->NotificationText = FText::FromString(TEXT("Connected to the server."));
		
		AllPlayerControllers.Add(PC);

		ABCPlayerState* BCPS = PC->GetPlayerState<ABCPlayerState>();
		if (IsValid(BCPS))
		{
			BCPS->PlayerName = TEXT("Player ") + FString::FromInt(AllPlayerControllers.Num());
		}

		ABCGameStateBase* BCGameStateBase = GetGameState<ABCGameStateBase>();
		if (IsValid(BCGameStateBase))
		{
			BCGameStateBase->MulticastRPCBroadcastLogin(BCPS->PlayerName);
		}
	}

	if (AllPlayerControllers.Num() >= 2)
	{
		StartGame();
	}
}

void ABCGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumber = GenerateSecretNumber();
}

void ABCGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorldTimerManager().ClearAllTimersForObject(this);
}

FString ABCGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 0; i < 10; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());

	FString Result;
	for (int32 i = 0; i < GuessNumberCount; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result += FString::FromInt(Numbers[Index]);
		Numbers.RemoveAt(Index);
	}

	UE_LOG(LogTemp, Log, TEXT("Secret Number: %s"), *Result);

	return Result;
}

bool ABCGameModeBase::IsGuessNumber(const FString& InGuessNumber)
{
	if (InGuessNumber.Len() != GuessNumberCount)
	{
		return false;
	}

	bool bIsUnique = true;
	TSet<TCHAR> UniqueDigits;
	for (TCHAR C : InGuessNumber)
	{
		if (!FChar::IsDigit(C) || UniqueDigits.Contains(C))
		{
			bIsUnique = false;
			break;
		}

		UniqueDigits.Add(C);
	}

	if (!bIsUnique)
	{
		return false;
	}

	return true;
}

FString ABCGameModeBase::JudgeResult(const FString& InSecretNumber, const FString& InGuessNumber)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < GuessNumberCount; ++i)
	{
		if (InSecretNumber[i] == InGuessNumber[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessNumber = FString::Printf(TEXT("%c"), InGuessNumber[i]);
			if (InSecretNumber.Contains(PlayerGuessNumber))
			{
				BallCount++;
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	UE_LOG(LogTemp,Warning, TEXT("StrikeCount: %d, BallCount: %d"), StrikeCount, BallCount);

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void ABCGameModeBase::PrintChatMessage(ABCPlayerController* InChattingPC, const FString& InChatMessage)
{
	FString ChatMessage = InChatMessage;
	int Index = ChatMessage.Len() - GuessNumberCount;
	FString GuessNumber = ChatMessage.RightChop(Index);

	ABCPlayerState* BCPS = InChattingPC->GetPlayerState<ABCPlayerState>();
	FString UpdatedMessage = InChatMessage;
	if (IsValid(BCPS))
	{
		UpdatedMessage = BCPS->GetPlayerInfo() + TEXT(": ") + InChatMessage;
	}
	
	if (BCPS->bIsTurn && BCPS->CurrentGuessCount < BCPS->MaxGuessCount && IsGuessNumber(GuessNumber))
	{
		FString JudgeResultMessage = JudgeResult(SecretNumber, GuessNumber);

		IncreaseGuessCount(InChattingPC);

		if (IsValid(BCPS))
		{
			UpdatedMessage = BCPS->GetPlayerInfo() + TEXT(": ") + InChatMessage;
		}

		for (ABCPlayerController* BCPC : AllPlayerControllers)
		{
			if (IsValid(BCPC))
			{
				FString CombinedMessage = UpdatedMessage + TEXT(" -> ") + JudgeResultMessage;
				BCPC->ClientRPCPrintChatMessage(CombinedMessage);
			}
		}

		ChangePlayerStateTurn(InChattingPC, false);
		CurrentPlayerIndex = (CurrentPlayerIndex + 1) % AllPlayerControllers.Num();
		ChangeTurn();
		
		const int32 StrikeCount = FCString::Atoi(*JudgeResultMessage.Left(1));
		JudgeGame(InChattingPC, StrikeCount);
	}
	else
	{
		for (ABCPlayerController* BCPC : AllPlayerControllers)
		{
			if (IsValid(BCPC))
			{
				BCPC->ClientRPCPrintChatMessage(UpdatedMessage);
			}
		}
	}
	
}

void ABCGameModeBase::IncreaseGuessCount(ABCPlayerController* InChattingPC)
{
	ABCPlayerState* BCPS = InChattingPC->GetPlayerState<ABCPlayerState>();
	if (IsValid(BCPS))
	{
		BCPS->CurrentGuessCount++;
	}
}

void ABCGameModeBase::ResetGame()
{
	SecretNumber = GenerateSecretNumber();

	for (ABCPlayerController* BCPC : AllPlayerControllers)
	{
		ABCPlayerState* BCPS = BCPC->GetPlayerState<ABCPlayerState>();
		if (IsValid(BCPS))
		{
			BCPS->CurrentGuessCount = 0;
		}
		
		BCPC->RemainingTime = 0;
	}

	StartGame();
}

bool ABCGameModeBase::JudgeGame(ABCPlayerController* InChattingPC, int InStrikeCount)
{
	bool bIsEnd = false;
	if (InStrikeCount == 3)
	{
		ABCPlayerState* BCPS = InChattingPC->GetPlayerState<ABCPlayerState>();
		for (const auto& PC : AllPlayerControllers)
		{
			if (IsValid(PC))
			{
				FString CombinedMessage = BCPS->PlayerName + TEXT(" winned!");
				PC->NotificationText = FText::FromString(CombinedMessage);
			}
		}

		bIsEnd = true;
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& PC : AllPlayerControllers)
		{
			ABCPlayerState* BCPS = PC->GetPlayerState<ABCPlayerState>();
			if (IsValid(BCPS))
			{
				if (BCPS->CurrentGuessCount < BCPS->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}

		if (bIsDraw)
		{
			for (const auto& PC : AllPlayerControllers)
			{
				PC->NotificationText = FText::FromString(TEXT("Draw!"));
			}
			
			bIsEnd = true;
		}
	}

	if (bIsEnd)
	{
		ResetGame();
	}

	return bIsEnd;
}

void ABCGameModeBase::StartGame()
{
	UE_LOG(LogTemp, Log, TEXT("Start Game!"));
	CurrentPlayerIndex = 0;
	ChangeTurn();
}

void ABCGameModeBase::ChangeTurn()
{
	GetWorldTimerManager().ClearTimer(TurnTimeUpdateHandle);
	RemainingTurnTime = 10.f;
	
	if (AllPlayerControllers.IsValidIndex(CurrentPlayerIndex))
	{
		ABCPlayerController* BCPC = AllPlayerControllers[CurrentPlayerIndex];
		if (IsValid(BCPC))
		{
			ChangePlayerStateTurn(BCPC, true);
		}
		
		GetWorldTimerManager().SetTimer(TurnTimeUpdateHandle, this, &ThisClass::UpdateRemainingTime, 0.1f, true);
	}
}

void ABCGameModeBase::UpdateRemainingTime()
{
	RemainingTurnTime = FMath::Max(0.0f, RemainingTurnTime - 0.1f);

	if (AllPlayerControllers.IsValidIndex(CurrentPlayerIndex))
	{
		ABCPlayerController* BCPC = AllPlayerControllers[CurrentPlayerIndex];
		BCPC->RemainingTime = RemainingTurnTime;
	}
			
	if (RemainingTurnTime <= 0.0f)
	{
		TurnTimeExpired();
	}
}

void ABCGameModeBase::TurnTimeExpired()
{
	if (AllPlayerControllers.IsValidIndex(CurrentPlayerIndex))
	{
		ABCPlayerController* BCPC = AllPlayerControllers[CurrentPlayerIndex];
		if (IsValid(BCPC))
		{
			IncreaseGuessCount(BCPC);
			ChangePlayerStateTurn(BCPC, false);
			
			if (!JudgeGame(BCPC, 0))
			{
				CurrentPlayerIndex = (CurrentPlayerIndex + 1) % AllPlayerControllers.Num();
				ChangeTurn();
			}
		}
	}
}

void ABCGameModeBase::ChangePlayerStateTurn(ABCPlayerController* InChattingPC, bool bInIsTurn)
{
	if (IsValid(InChattingPC))
	{
		ABCPlayerState* BCPS = InChattingPC->GetPlayerState<ABCPlayerState>();
		if (IsValid(BCPS))
		{
			BCPS->bIsTurn = bInIsTurn;
		}

		if (!bInIsTurn)
		{
			InChattingPC->RemainingTime = 0;
		}
		else
		{
			InChattingPC->RemainingTime = RemainingTurnTime;
		}
	}
}
