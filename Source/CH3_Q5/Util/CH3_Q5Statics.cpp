#include "CH3_Q5/Util/CH3_Q5Statics.h"

#include "CH3_Q5/Subsystem/GameManagerSubsystem.h"
#include "CH3_Q5/Subsystem/ItemSpawnSubsystem.h"
#include "CH3_Q5/Wave/GameStateWave.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"

UItemSpawnSubsystem* UCH3_Q5Statics::GetItemSpawnSubsystem(const UObject* WorldContextObject)
{
	if (!GEngine || !WorldContextObject)
	{
		return nullptr;
	}

	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull))
	{
		return World->GetSubsystem<UItemSpawnSubsystem>();
	}

	return nullptr;
}

AGameStateWave* UCH3_Q5Statics::GetWaveGameState(const UObject* WorldContextObject)
{
	if (!GEngine || !WorldContextObject)
	{
		return nullptr;
	}

	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull))
	{
		return World->GetGameState<AGameStateWave>();
	}

	return nullptr;
}

UGameManagerSubsystem* UCH3_Q5Statics::GetGameManagerSubsystem(const UObject* WorldContextObject)
{
	if (!GEngine || !WorldContextObject)
	{
		return nullptr;
	}

	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull))
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			return GameInstance->GetSubsystem<UGameManagerSubsystem>();
		}
	}

	return nullptr;
}
