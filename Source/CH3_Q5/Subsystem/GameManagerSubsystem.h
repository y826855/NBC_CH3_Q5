#pragma once

#include "CoreMinimal.h"
#include "CH3_Q5/Game/GameFinishResult.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TimerManager.h"
#include "GameManagerSubsystem.generated.h"

class AGameStateWave;
class UHealthComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameManagerSubsystemEventSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameManagerSubsystemFinishedSignature, EGameFinishResult, FinishResult);
DECLARE_DYNAMIC_DELEGATE_OneParam(FGameFinishedEventSignature, EGameFinishResult, FinishResult);

USTRUCT(BlueprintType)
struct FWaveLevelConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	FName LevelName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float WaveTime = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float SpawnInterval = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 TargetScore = 0;
};

UCLASS()
class CH3_Q5_API UGameManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UGameManagerSubsystem();

	virtual void Deinitialize() override;

	UPROPERTY(BlueprintAssignable, Category = "Game")
	FOnGameManagerSubsystemEventSignature OnGameStarted;

	UPROPERTY(BlueprintAssignable, Category = "Game")
	FOnGameManagerSubsystemFinishedSignature OnGameFinished;

	UPROPERTY(BlueprintAssignable, Category = "Game")
	FOnGameManagerSubsystemEventSignature OnWaveCompleted;

	UFUNCTION(BlueprintCallable, Category = "Game")
	void StartGame();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void FinishGame(EGameFinishResult FinishResult = EGameFinishResult::Cleared);

	UFUNCTION(BlueprintCallable, Category = "Game")
	void CompleteCurrentWave();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void ResetProgress();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void OpenCurrentWaveLevel();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void SetWaveConfigs(const TArray<FWaveLevelConfig>& NewWaveConfigs);

	UFUNCTION(BlueprintCallable, Category = "Game|Score")
	void AddScore(int32 Amount);

	UFUNCTION(BlueprintPure, Category = "Game|Score")
	int32 GetScore() const { return Score; }

	UFUNCTION(BlueprintCallable, Category = "Game|Event")
	void BindGameFinishedEvent(const FGameFinishedEventSignature& Event);

	UFUNCTION(BlueprintCallable, Category = "Game|Event")
	void UnbindGameFinishedEventsForObject(UObject* Object);

	UFUNCTION(BlueprintPure, Category = "Game")
	int32 GetCurrentWaveIndex() const { return CurrentWaveIndex + 1; }

	UFUNCTION(BlueprintPure, Category = "Game")
	int32 GetWaveCount() const { return WaveConfigs.Num(); }

	UFUNCTION(BlueprintPure, Category = "Game")
	FWaveLevelConfig GetCurrentWaveConfig() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
	TArray<FWaveLevelConfig> WaveConfigs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
	float TimerTickInterval = 1.0f;

private:
	int32 CurrentWaveIndex = 0;
	int32 Score = 0;
	FTimerHandle GameTimerHandle;

	TArray<FGameFinishedEventSignature> GameFinishedEvents;

	TWeakObjectPtr<AGameStateWave> CachedGameState;
	TWeakObjectPtr<UHealthComponent> BoundPlayerHealthComponent;

	void StartGameTimer();
	void StopGameTimer();
	void HandleGameTimerTick();
	void BindGameState();
	void UnbindGameState();
	void BindPlayerHealthComponent();
	void UnbindPlayerHealthComponent();
	void ApplyCurrentWaveSettings();
	void ApplyScoreToGameState();
	void BroadcastGameFinished(EGameFinishResult FinishResult);

	UFUNCTION()
	void HandlePlayerHealthDepleted();
};
