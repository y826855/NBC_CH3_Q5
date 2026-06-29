#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "HUDViewModel.generated.h"

class AGameStateWave;
class UHealthComponent;

UCLASS(BlueprintType)
class CH3_Q5_API UHUDViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void Initialize(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void Deinitialize();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetHealth(float CurrentHealth, float MaxHealth);

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "HUD")
	FText RemainingTimeText;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "HUD")
	FText ScoreText;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "HUD")
	FText WaveText;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "HUD")
	FText HealthText;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "HUD")
	float HealthPercent = 1.0f;

	FText GetRemainingTimeText() const { return RemainingTimeText; }
	FText GetScoreText() const { return ScoreText; }
	FText GetWaveText() const { return WaveText; }
	FText GetHealthText() const { return HealthText; }
	float GetHealthPercent() const { return HealthPercent; }

protected:
	void SetRemainingTimeText(const FText& NewText);
	void SetScoreText(const FText& NewText);
	void SetWaveText(const FText& NewText);
	void SetHealthText(const FText& NewText);
	void SetHealthPercent(float NewPercent);

private:
	UPROPERTY()
	TObjectPtr<AGameStateWave> CachedGameState;

	UPROPERTY()
	TObjectPtr<UHealthComponent> CachedHealthComponent;

	UFUNCTION()
	void HandleRemainingTimeChanged(float NewRemainingTime);

	UFUNCTION()
	void HandleScoreChanged(int32 NewScore);

	UFUNCTION()
	void HandleWaveChanged(int32 NewWaveIndex, float NewWaveTime);

	UFUNCTION()
	void HandleHealthChanged(float CurrentHealth, float MaxHealth);

	void BindToGameState(AGameStateWave* NewGameState);
	void UnbindFromGameState();
	void BindToPlayerHealth(UObject* WorldContextObject);
	void UnbindFromPlayerHealth();
	void RefreshFromGameState();
	void RefreshFromHealthComponent();
};
