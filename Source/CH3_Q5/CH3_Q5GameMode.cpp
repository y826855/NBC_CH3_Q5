// Copyright Epic Games, Inc. All Rights Reserved.

#include "CH3_Q5GameMode.h"

#include "CH3_Q5/Wave/GameStateWave.h"

ACH3_Q5GameMode::ACH3_Q5GameMode()
{
	GameStateClass = AGameStateWave::StaticClass();
}
