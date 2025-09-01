// BCGameModeBase.h

#pragma once

#include "GameFramework/GameModeBase.h"
#include "BCGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BULLSANDCOWS_API ABCGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;
};
