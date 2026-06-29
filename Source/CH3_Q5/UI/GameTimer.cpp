// Fill out your copyright notice in the Description page of Project Settings.


#include "GameTimer.h"

#include "CH3_Q5.h"
#include "CH3_Q5/Util/CH3_Q5Statics.h"
#include "CH3_Q5/Wave/GameStateWave.h"
#include "Components/TextBlock.h"

void UGameTimer::NativeConstruct()
{
	Super::NativeConstruct();

	BindToGameState();
	RefreshFromGameState();
}

void UGameTimer::NativeDestruct()
{
	UnbindFromGameState();

	Super::NativeDestruct();
}

void UGameTimer::RefreshFromGameState()
{
	BindToGameState();

	if (CachedGameState)
	{
		SetRemainingTimeText(CachedGameState->GetRemainingTime());
	}
}

void UGameTimer::HandleRemainingTimeChanged(float NewRemainingTime)
{
	SetRemainingTimeText(NewRemainingTime);
}

void UGameTimer::BindToGameState()
{
	if (CachedGameState)
	{
		return;
	}

	CachedGameState = UCH3_Q5Statics::GetWaveGameState(this);
	if (!CachedGameState)
	{
		return;
	}

	CachedGameState->OnRemainingTimeChanged.RemoveDynamic(this, &UGameTimer::HandleRemainingTimeChanged);
	CachedGameState->OnRemainingTimeChanged.AddDynamic(this, &UGameTimer::HandleRemainingTimeChanged);
}

void UGameTimer::UnbindFromGameState()
{
	if (CachedGameState)
	{
		CachedGameState->OnRemainingTimeChanged.RemoveDynamic(this, &UGameTimer::HandleRemainingTimeChanged);
		CachedGameState = nullptr;
	}
}

void UGameTimer::SetRemainingTimeText(float NewRemainingTime)
{
	if (!RemainingTimeText)
	{
		return;
	}

	const int32 DisplaySeconds = FMath::CeilToInt(FMath::Max(0.0f, NewRemainingTime));
	RemainingTimeText->SetText(FText::AsNumber(DisplaySeconds));
}
