// BCGameModeBase.h

#pragma once

#include "GameFramework/GameModeBase.h"
#include "BCGameModeBase.generated.h"

class ABCPlayerController;
/**
 * 
 */
UCLASS()
class BULLSANDCOWS_API ABCGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FString GenerateSecretNumber();
	bool IsGuessNumber(const FString& InGuessNumber);
	FString JudgeResult(const FString& InSecretNumber, const FString& InGuessNumber);

	void PrintChatMessage(ABCPlayerController* InChattingPC, const FString& InChatMessage);
	
	void IncreaseGuessCount(ABCPlayerController* InChattingPC);
	void ResetGame();
	bool JudgeGame(ABCPlayerController* InChattingPC, int InStrikeCount);

	void StartGame();
	void ChangeTurn();
	void TurnTimeExpired();
	
protected:
	FString SecretNumber;
	int32 GuessNumberCount = 3;

	TArray<TObjectPtr<ABCPlayerController>> AllPlayerControllers;

	int32 CurrentPlayerIndex;

	float RemainingTurnTime = 10.f;
	FTimerHandle TurnTimeUpdateHandle;

private:
	void UpdateRemainingTime();
	void ChangePlayerStateTurn(ABCPlayerController* InChattingPC, bool bInIsTurn);
	
};
