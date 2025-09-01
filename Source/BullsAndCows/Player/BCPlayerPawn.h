// BCPlayerPawn.h

#pragma once

#include "GameFramework/Pawn.h"
#include "BCPlayerPawn.generated.h"

UCLASS()
class BULLSANDCOWS_API ABCPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	ABCPlayerPawn();

protected:
	virtual void BeginPlay() override;
};
