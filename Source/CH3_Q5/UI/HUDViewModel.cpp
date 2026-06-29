#include "CH3_Q5/UI/HUDViewModel.h"

#include "CH3_Q5/Character/Component/HealthComponent.h"
#include "CH3_Q5/Util/CH3_Q5Statics.h"
#include "CH3_Q5/Wave/GameStateWave.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

void UHUDViewModel::Initialize(UObject* WorldContextObject)
{
	BindToGameState(UCH3_Q5Statics::GetWaveGameState(WorldContextObject));
	BindToPlayerHealth(WorldContextObject);
	RefreshFromGameState();
	RefreshFromHealthComponent();
}

void UHUDViewModel::Deinitialize()
{
	UnbindFromPlayerHealth();
	UnbindFromGameState();
}

void UHUDViewModel::SetHealth(float CurrentHealth, float MaxHealth)
{
	const float SafeMaxHealth = FMath::Max(0.0f, MaxHealth);
	const float ClampedCurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, SafeMaxHealth);
	const float NewHealthPercent = SafeMaxHealth > 0.0f ? ClampedCurrentHealth / SafeMaxHealth : 0.0f;

	SetHealthText(FText::Format(
		NSLOCTEXT("HUDViewModel", "HealthFormat", "{0} / {1}"),
		FText::AsNumber(FMath::RoundToInt(ClampedCurrentHealth)),
		FText::AsNumber(FMath::RoundToInt(SafeMaxHealth))
	));
	SetHealthPercent(NewHealthPercent);
}

void UHUDViewModel::SetRemainingTimeText(const FText& NewText)
{
	UE_MVVM_SET_PROPERTY_VALUE(RemainingTimeText, NewText);
}

void UHUDViewModel::SetScoreText(const FText& NewText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ScoreText, NewText);
}

void UHUDViewModel::SetWaveText(const FText& NewText)
{
	UE_MVVM_SET_PROPERTY_VALUE(WaveText, NewText);
}

void UHUDViewModel::SetHealthText(const FText& NewText)
{
	UE_MVVM_SET_PROPERTY_VALUE(HealthText, NewText);
}

void UHUDViewModel::SetHealthPercent(float NewPercent)
{
	UE_MVVM_SET_PROPERTY_VALUE(HealthPercent, FMath::Clamp(NewPercent, 0.0f, 1.0f));
}

void UHUDViewModel::HandleRemainingTimeChanged(float NewRemainingTime)
{
	const int32 DisplaySeconds = FMath::CeilToInt(FMath::Max(0.0f, NewRemainingTime));
	SetRemainingTimeText(FText::AsNumber(DisplaySeconds));
}

void UHUDViewModel::HandleScoreChanged(int32 NewScore)
{
	SetScoreText(FText::AsNumber(NewScore));
}

void UHUDViewModel::HandleWaveChanged(int32 NewWaveIndex, float NewWaveTime)
{
	SetWaveText(FText::AsNumber(NewWaveIndex));
}

void UHUDViewModel::HandleHealthChanged(float CurrentHealth, float MaxHealth)
{
	SetHealth(CurrentHealth, MaxHealth);
}

void UHUDViewModel::BindToGameState(AGameStateWave* NewGameState)
{
	if (CachedGameState == NewGameState)
	{
		return;
	}

	UnbindFromGameState();
	CachedGameState = NewGameState;

	if (!CachedGameState)
	{
		return;
	}

	CachedGameState->OnRemainingTimeChanged.AddDynamic(this, &UHUDViewModel::HandleRemainingTimeChanged);
	CachedGameState->OnScoreChanged.AddDynamic(this, &UHUDViewModel::HandleScoreChanged);
	CachedGameState->OnWaveChanged.AddDynamic(this, &UHUDViewModel::HandleWaveChanged);
}

void UHUDViewModel::UnbindFromGameState()
{
	if (!CachedGameState)
	{
		return;
	}

	CachedGameState->OnRemainingTimeChanged.RemoveDynamic(this, &UHUDViewModel::HandleRemainingTimeChanged);
	CachedGameState->OnScoreChanged.RemoveDynamic(this, &UHUDViewModel::HandleScoreChanged);
	CachedGameState->OnWaveChanged.RemoveDynamic(this, &UHUDViewModel::HandleWaveChanged);
	CachedGameState = nullptr;
}

void UHUDViewModel::BindToPlayerHealth(UObject* WorldContextObject)
{
	if (CachedHealthComponent || !GEngine || !WorldContextObject)
	{
		return;
	}

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!World)
	{
		return;
	}

	APawn* PlayerPawn = World->GetFirstPlayerController() ? World->GetFirstPlayerController()->GetPawn() : nullptr;
	if (!PlayerPawn)
	{
		return;
	}

	CachedHealthComponent = PlayerPawn->FindComponentByClass<UHealthComponent>();
	if (!CachedHealthComponent)
	{
		return;
	}

	CachedHealthComponent->OnHealthChanged.RemoveDynamic(this, &UHUDViewModel::HandleHealthChanged);
	CachedHealthComponent->OnHealthChanged.AddDynamic(this, &UHUDViewModel::HandleHealthChanged);
}

void UHUDViewModel::UnbindFromPlayerHealth()
{
	if (!CachedHealthComponent)
	{
		return;
	}

	CachedHealthComponent->OnHealthChanged.RemoveDynamic(this, &UHUDViewModel::HandleHealthChanged);
	CachedHealthComponent = nullptr;
}

void UHUDViewModel::RefreshFromGameState()
{
	if (!CachedGameState)
	{
		SetRemainingTimeText(FText::AsNumber(0));
		SetScoreText(FText::AsNumber(0));
		SetWaveText(FText::AsNumber(0));
		return;
	}

	HandleRemainingTimeChanged(CachedGameState->GetRemainingTime());
	HandleScoreChanged(CachedGameState->GetScore());
	HandleWaveChanged(CachedGameState->GetCurrentWaveIndex(), CachedGameState->GetWaveTime());
}

void UHUDViewModel::RefreshFromHealthComponent()
{
	if (!CachedHealthComponent)
	{
		SetHealth(100.0f, 100.0f);
		return;
	}

	SetHealth(CachedHealthComponent->GetCurrentHealth(), CachedHealthComponent->GetMaxHealth());
}
