// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CH3_Q5GameMode.generated.h"

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class ACH3_Q5GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	ACH3_Q5GameMode();
};



