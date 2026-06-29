// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateWave.h"

AGameStateWave::AGameStateWave()
{
	RemainingTime = WaveTime;
}

void AGameStateWave::BeginPlay()
{
	Super::BeginPlay();
}

void AGameStateWave::ResetGameState()
{
	Score = 0;
	RemainingTime = WaveTime;
	CurrentWaveIndex = 0;
	GameFlowState = EWaveGameFlowState::BeforeStart;

	OnScoreChanged.Broadcast(Score);
	OnRemainingTimeChanged.Broadcast(RemainingTime);
	OnWaveChanged.Broadcast(CurrentWaveIndex, WaveTime);
	OnGameFlowStateChanged.Broadcast(GameFlowState);
}

void AGameStateWave::SetGameFlowState(EWaveGameFlowState NewState)
{
	if (GameFlowState == NewState)
	{
		return;
	}

	GameFlowState = NewState;
	OnGameFlowStateChanged.Broadcast(GameFlowState);
}

void AGameStateWave::SetCurrentWave(int32 NewWaveIndex, float NewWaveTime)
{
	CurrentWaveIndex = FMath::Max(0, NewWaveIndex);
	WaveTime = FMath::Max(0.0f, NewWaveTime);
	RemainingTime = WaveTime;

	OnWaveChanged.Broadcast(CurrentWaveIndex, WaveTime);
	OnRemainingTimeChanged.Broadcast(RemainingTime);
}

void AGameStateWave::AddScore(int32 Amount)
{
	if (Amount <= 0)
	{
		return;
	}

	Score += Amount;
	OnScoreChanged.Broadcast(Score);
}

void AGameStateWave::SetScore(int32 NewScore)
{
	Score = FMath::Max(0, NewScore);
	OnScoreChanged.Broadcast(Score);
}

void AGameStateWave::SetRemainingTime(float NewRemainingTime)
{
	RemainingTime = FMath::Max(0.0f, NewRemainingTime);
	OnRemainingTimeChanged.Broadcast(RemainingTime);
}
