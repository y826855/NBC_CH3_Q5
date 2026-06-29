#include "CH3_Q5/Subsystem/GameManagerSubsystem.h"

#include "CH3_Q5.h"
#include "CH3_Q5/Character/Component/HealthComponent.h"
#include "CH3_Q5/Subsystem/ItemSpawnSubsystem.h"
#include "CH3_Q5/Wave/GameStateWave.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

UGameManagerSubsystem::UGameManagerSubsystem()
{
	WaveConfigs.Add({ FName("Lvl_Wave01"), 30.0f, 2.0f, 0 });
	WaveConfigs.Add({ FName("Lvl_Wave02"), 35.0f, 1.5f, 0 });
	WaveConfigs.Add({ FName("Lvl_Wave03"), 40.0f, 1.0f, 0 });
}

void UGameManagerSubsystem::Deinitialize()
{
	UnbindGameState();
	UnbindPlayerHealthComponent();
	StopGameTimer();

	Super::Deinitialize();
}

void UGameManagerSubsystem::StartGame()
{
	BindGameState();

	AGameStateWave* WaveGameState = CachedGameState.Get();
	if (!WaveGameState)
	{
		return;
	}

	WaveGameState->ResetGameState();
	ApplyCurrentWaveSettings();
	ApplyScoreToGameState();

	BindPlayerHealthComponent();
	if (BoundPlayerHealthComponent.IsValid())
	{
		BoundPlayerHealthComponent->ResetHealth();
	}

	WaveGameState->SetGameFlowState(EWaveGameFlowState::Playing);
	StartGameTimer();
	OnGameStarted.Broadcast();
}

void UGameManagerSubsystem::FinishGame(EGameFinishResult FinishResult)
{
	AGameStateWave* WaveGameState = CachedGameState.Get();
	if (!WaveGameState)
	{
		StopGameTimer();
		UnbindPlayerHealthComponent();
		return;
	}

	if (WaveGameState->GetGameFlowState() == EWaveGameFlowState::Finished)
	{
		return;
	}

	UnbindGameState();
	UnbindPlayerHealthComponent();
	WaveGameState->SetRemainingTime(0.0f);
	WaveGameState->SetGameFlowState(EWaveGameFlowState::Finished);
	StopGameTimer();
	BroadcastGameFinished(FinishResult);
}

void UGameManagerSubsystem::CompleteCurrentWave()
{
	OnWaveCompleted.Broadcast();

	if (CurrentWaveIndex >= WaveConfigs.Num() - 1)
	{
		FinishGame(EGameFinishResult::Cleared);
		return;
	}

	UnbindGameState();
	UnbindPlayerHealthComponent();
	StopGameTimer();

	++CurrentWaveIndex;
	OpenCurrentWaveLevel();
}

void UGameManagerSubsystem::ResetProgress()
{
	CurrentWaveIndex = 0;
	Score = 0;
}

void UGameManagerSubsystem::OpenCurrentWaveLevel()
{
	const FWaveLevelConfig CurrentWaveConfig = GetCurrentWaveConfig();
	if (CurrentWaveConfig.LevelName.IsNone())
	{
		return;
	}

	if (UWorld* World = GetWorld())
	{
		UGameplayStatics::OpenLevel(World, CurrentWaveConfig.LevelName);
	}
}

void UGameManagerSubsystem::SetWaveConfigs(const TArray<FWaveLevelConfig>& NewWaveConfigs)
{
	WaveConfigs = NewWaveConfigs;
	CurrentWaveIndex = FMath::Clamp(CurrentWaveIndex, 0, FMath::Max(0, WaveConfigs.Num() - 1));
}

void UGameManagerSubsystem::AddScore(int32 Amount)
{
	if (Amount <= 0)
	{
		return;
	}

	Score += Amount;
	ApplyScoreToGameState();

	const FWaveLevelConfig CurrentWaveConfig = GetCurrentWaveConfig();
	if (CurrentWaveConfig.TargetScore > 0 && Score >= CurrentWaveConfig.TargetScore)
	{
		CompleteCurrentWave();
	}
}

void UGameManagerSubsystem::BindGameFinishedEvent(const FGameFinishedEventSignature& Event)
{
	if (Event.IsBound())
	{
		GameFinishedEvents.Add(Event);
	}
}

void UGameManagerSubsystem::UnbindGameFinishedEventsForObject(UObject* Object)
{
	if (Object)
	{
		OnGameFinished.RemoveAll(Object);
		GameFinishedEvents.RemoveAll([Object](const FGameFinishedEventSignature& Event)
		{
			return Event.IsBoundToObject(Object);
		});
	}
}

FWaveLevelConfig UGameManagerSubsystem::GetCurrentWaveConfig() const
{
	if (WaveConfigs.IsValidIndex(CurrentWaveIndex))
	{
		return WaveConfigs[CurrentWaveIndex];
	}

	return FWaveLevelConfig();
}

void UGameManagerSubsystem::StartGameTimer()
{
	if (TimerTickInterval <= 0.0f)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	World->GetTimerManager().SetTimer(
		GameTimerHandle,
		this,
		&UGameManagerSubsystem::HandleGameTimerTick,
		TimerTickInterval,
		true
	);
}

void UGameManagerSubsystem::StopGameTimer()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(GameTimerHandle);
	}
}

void UGameManagerSubsystem::HandleGameTimerTick()
{
	AGameStateWave* WaveGameState = CachedGameState.Get();
	if (!WaveGameState || WaveGameState->GetGameFlowState() != EWaveGameFlowState::Playing)
	{
		StopGameTimer();
		return;
	}

	const float NewRemainingTime = WaveGameState->GetRemainingTime() - TimerTickInterval;
	WaveGameState->SetRemainingTime(NewRemainingTime);

	if (NewRemainingTime > 0.0f)
	{
		return;
	}

	CompleteCurrentWave();
}

void UGameManagerSubsystem::BindGameState()
{
	AGameStateWave* NewGameState = nullptr;
	if (UWorld* World = GetWorld())
	{
		NewGameState = World->GetGameState<AGameStateWave>();
	}

	if (CachedGameState.Get() == NewGameState)
	{
		return;
	}

	UnbindGameState();
	CachedGameState = NewGameState;

	if (CachedGameState.IsValid())
	{
		ApplyScoreToGameState();
	}
}

void UGameManagerSubsystem::UnbindGameState()
{
	if (CachedGameState.IsValid())
	{
		CachedGameState.Reset();
	}
}

void UGameManagerSubsystem::BindPlayerHealthComponent()
{
	UnbindPlayerHealthComponent();

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PlayerPawn)
	{
		return;
	}

	UHealthComponent* HealthComponent = PlayerPawn->FindComponentByClass<UHealthComponent>();
	if (!HealthComponent)
	{
		return;
	}

	HealthComponent->OnHealthDepleted.AddUniqueDynamic(this, &UGameManagerSubsystem::HandlePlayerHealthDepleted);
	BoundPlayerHealthComponent = HealthComponent;
}

void UGameManagerSubsystem::UnbindPlayerHealthComponent()
{
	if (BoundPlayerHealthComponent.IsValid())
	{
		BoundPlayerHealthComponent->OnHealthDepleted.RemoveDynamic(this, &UGameManagerSubsystem::HandlePlayerHealthDepleted);
		BoundPlayerHealthComponent.Reset();
	}
}

void UGameManagerSubsystem::ApplyCurrentWaveSettings()
{
	AGameStateWave* WaveGameState = CachedGameState.Get();
	if (!WaveGameState)
	{
		return;
	}

	const FWaveLevelConfig CurrentWaveConfig = GetCurrentWaveConfig();
	WaveGameState->SetCurrentWave(CurrentWaveIndex + 1, CurrentWaveConfig.WaveTime);

	if (UWorld* World = GetWorld())
	{
		if (UItemSpawnSubsystem* ItemSpawnSubsystem = World->GetSubsystem<UItemSpawnSubsystem>())
		{
			ItemSpawnSubsystem->SetSpawnInterval(CurrentWaveConfig.SpawnInterval);
		}
	}
}

void UGameManagerSubsystem::ApplyScoreToGameState()
{
	if (CachedGameState.IsValid())
	{
		CachedGameState->SetScore(Score);
	}
}

void UGameManagerSubsystem::BroadcastGameFinished(EGameFinishResult FinishResult)
{
	OnGameFinished.Broadcast(FinishResult);

	for (const FGameFinishedEventSignature& Event : GameFinishedEvents)
	{
		if (Event.IsBound())
		{
			Event.Execute(FinishResult);
		}
	}
}

void UGameManagerSubsystem::HandlePlayerHealthDepleted()
{
	FinishGame(EGameFinishResult::Failed);
}
