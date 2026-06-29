#include "CH3_Q5/Subsystem/ItemSpawnSubsystem.h"

#include "CH3_Q5.h"
#include "CH3_Q5/Spawner/SpawnVolume.h"
#include "CH3_Q5/Util/CH3_Q5Statics.h"
#include "Engine/World.h"

void UItemSpawnSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	BindToGameState();
	SyncSpawnTimerWithGameState();
}

void UItemSpawnSubsystem::Deinitialize()
{
	StopSpawnTimer();

	if (IsValid(CachedGameState))
	{
		CachedGameState->OnGameFlowStateChanged.RemoveDynamic(this, &UItemSpawnSubsystem::HandleGameFlowStateChanged);
	}

	CachedGameState = nullptr;
	SpawnVolumes.Reset();

	Super::Deinitialize();
}

void UItemSpawnSubsystem::RegisterSpawnVolume(ASpawnVolume* SpawnVolume)
{
	if (!IsValid(SpawnVolume))
	{
		return;
	}

	RemoveInvalidSpawnVolumes();

	const bool bAlreadyRegistered = SpawnVolumes.ContainsByPredicate([SpawnVolume](const TWeakObjectPtr<ASpawnVolume>& RegisteredVolume)
	{
		return RegisteredVolume.Get() == SpawnVolume;
	});

	if (!bAlreadyRegistered)
	{
		SpawnVolumes.Add(SpawnVolume);
	}

	BindToGameState();
	SyncSpawnTimerWithGameState();
}

void UItemSpawnSubsystem::UnregisterSpawnVolume(ASpawnVolume* SpawnVolume)
{
	if (!SpawnVolume)
	{
		return;
	}

	SpawnVolumes.RemoveAll([SpawnVolume](const TWeakObjectPtr<ASpawnVolume>& RegisteredVolume)
	{
		return !RegisteredVolume.IsValid() || RegisteredVolume.Get() == SpawnVolume;
	});

}

void UItemSpawnSubsystem::SetSpawnInterval(float NewSpawnInterval)
{
	SpawnInterval = FMath::Max(0.0f, NewSpawnInterval);
	SyncSpawnTimerWithGameState();
}

void UItemSpawnSubsystem::SpawnRegisteredVolumesOnce()
{
	RemoveInvalidSpawnVolumes();

	for (const TWeakObjectPtr<ASpawnVolume>& SpawnVolume : SpawnVolumes)
	{
		if (SpawnVolume.IsValid())
		{
			SpawnVolume->SpawnRandomItem();
		}
	}
}

void UItemSpawnSubsystem::HandleGameFlowStateChanged(EWaveGameFlowState NewState)
{
	if (NewState == EWaveGameFlowState::Playing)
	{
		StartSpawnTimer();
		return;
	}

	StopSpawnTimer();
}

void UItemSpawnSubsystem::BindToGameState()
{
	if (CachedGameState)
	{
		return;
	}

	CachedGameState = UCH3_Q5Statics::GetWaveGameState(this);
	if (CachedGameState)
	{
		CachedGameState->OnGameFlowStateChanged.RemoveDynamic(this, &UItemSpawnSubsystem::HandleGameFlowStateChanged);
		CachedGameState->OnGameFlowStateChanged.AddDynamic(this, &UItemSpawnSubsystem::HandleGameFlowStateChanged);
	}
}

void UItemSpawnSubsystem::SyncSpawnTimerWithGameState()
{
	BindToGameState();

	if (CachedGameState && CachedGameState->GetGameFlowState() == EWaveGameFlowState::Playing)
	{
		StartSpawnTimer();
		return;
	}

	StopSpawnTimer();
}

void UItemSpawnSubsystem::StartSpawnTimer()
{
	if (SpawnInterval <= 0.0f)
	{
		StopSpawnTimer();
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	World->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&UItemSpawnSubsystem::HandleSpawnTimerTick,
		SpawnInterval,
		true
	);
}

void UItemSpawnSubsystem::StopSpawnTimer()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(SpawnTimerHandle);
	}
}

void UItemSpawnSubsystem::HandleSpawnTimerTick()
{
	if (!CachedGameState || CachedGameState->GetGameFlowState() != EWaveGameFlowState::Playing)
	{
		StopSpawnTimer();
		return;
	}

	SpawnRegisteredVolumesOnce();
}

void UItemSpawnSubsystem::RemoveInvalidSpawnVolumes()
{
	SpawnVolumes.RemoveAll([](const TWeakObjectPtr<ASpawnVolume>& SpawnVolume)
	{
		return !SpawnVolume.IsValid();
	});
}
