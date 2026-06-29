#include "CH3_Q5/Game/GameManager.h"

#include "CH3_Q5.h"
#include "CH3_Q5/Character/Component/HealthComponent.h"
#include "CH3_Q5/Util/CH3_Q5Statics.h"
#include "CH3_Q5/Wave/GameStateWave.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AGameManager::AGameManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGameManager::BeginPlay()
{
	Super::BeginPlay();
}

void AGameManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindPlayerHealthComponent();
	StopGameTimer();

	Super::EndPlay(EndPlayReason);
}

void AGameManager::StartGame()
{
	AGameStateWave* WaveGameState = GetWaveGameState();
	if (!WaveGameState)
	{
		return;
	}

	WaveGameState->ResetGameState();
	WaveGameState->SetCurrentWave(FirstWaveIndex, WaveGameState->GetWaveTime());
	BindPlayerHealthComponent();
	if (BoundPlayerHealthComponent.IsValid())
	{
		BoundPlayerHealthComponent->ResetHealth();
	}
	WaveGameState->SetGameFlowState(EWaveGameFlowState::Playing);
	StartGameTimer();
	OnGameStarted.Broadcast();
	ReceiveGameStarted();
}

void AGameManager::FinishGame(EGameFinishResult FinishResult)
{
	AGameStateWave* WaveGameState = GetWaveGameState();
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

	UnbindPlayerHealthComponent();
	WaveGameState->SetRemainingTime(0.0f);
	WaveGameState->SetGameFlowState(EWaveGameFlowState::Finished);
	StopGameTimer();
	OnGameFinished.Broadcast(FinishResult);
	ReceiveGameFinished(FinishResult);
}

AGameStateWave* AGameManager::GetWaveGameState() const
{
	return UCH3_Q5Statics::GetWaveGameState(this);
}

void AGameManager::StartGameTimer()
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
		&AGameManager::HandleGameTimerTick,
		TimerTickInterval,
		true
	);
}

void AGameManager::StopGameTimer()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(GameTimerHandle);
	}
}

void AGameManager::HandleGameTimerTick()
{
	AGameStateWave* WaveGameState = GetWaveGameState();
	if (!WaveGameState || WaveGameState->GetGameFlowState() != EWaveGameFlowState::Playing)
	{
		StopGameTimer();
		return;
	}

	const float NewRemainingTime = WaveGameState->GetRemainingTime() - TimerTickInterval;
	WaveGameState->SetRemainingTime(NewRemainingTime);

	if (NewRemainingTime <= 0.0f)
	{
		FinishGame(EGameFinishResult::Cleared);
	}
}

void AGameManager::BindPlayerHealthComponent()
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

	HealthComponent->OnHealthDepleted.AddUniqueDynamic(this, &AGameManager::HandlePlayerHealthDepleted);
	BoundPlayerHealthComponent = HealthComponent;
}

void AGameManager::UnbindPlayerHealthComponent()
{
	if (BoundPlayerHealthComponent.IsValid())
	{
		BoundPlayerHealthComponent->OnHealthDepleted.RemoveDynamic(this, &AGameManager::HandlePlayerHealthDepleted);
		BoundPlayerHealthComponent.Reset();
	}
}

void AGameManager::HandlePlayerHealthDepleted()
{
	FinishGame(EGameFinishResult::Failed);
}
