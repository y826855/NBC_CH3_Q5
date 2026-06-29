// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameStateWave.generated.h"

UENUM(BlueprintType)
enum class EWaveGameFlowState : uint8
{
	BeforeStart UMETA(DisplayName = "Before Start"),
	Playing UMETA(DisplayName = "Playing"),
	Finished UMETA(DisplayName = "Finished"),
	Waiting UMETA(DisplayName = "Waiting")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChangedSignature, int32, NewScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemainingTimeChangedSignature, float, NewRemainingTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWaveChangedSignature, int32, NewWaveIndex, float, NewWaveTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameFlowStateChangedSignature, EWaveGameFlowState, NewState);

UCLASS()
class CH3_Q5_API AGameStateWave : public AGameStateBase
{
	GENERATED_BODY()

public:
	AGameStateWave();

	UPROPERTY(BlueprintAssignable, Category = "Wave|Event")
	FOnScoreChangedSignature OnScoreChanged;

	UPROPERTY(BlueprintAssignable, Category = "Wave|Event")
	FOnRemainingTimeChangedSignature OnRemainingTimeChanged;

	UPROPERTY(BlueprintAssignable, Category = "Wave|Event")
	FOnWaveChangedSignature OnWaveChanged;

	UPROPERTY(BlueprintAssignable, Category = "Wave|Event")
	FOnGameFlowStateChangedSignature OnGameFlowStateChanged;

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void ResetGameState();

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void SetGameFlowState(EWaveGameFlowState NewState);

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void SetCurrentWave(int32 NewWaveIndex, float NewWaveTime);

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void AddScore(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void SetScore(int32 NewScore);

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void SetRemainingTime(float NewRemainingTime);

	UFUNCTION(BlueprintPure, Category = "Wave")
	int32 GetScore() const { return Score; }

	UFUNCTION(BlueprintPure, Category = "Wave")
	float GetRemainingTime() const { return RemainingTime; }

	UFUNCTION(BlueprintPure, Category = "Wave")
	int32 GetCurrentWaveIndex() const { return CurrentWaveIndex; }

	UFUNCTION(BlueprintPure, Category = "Wave")
	float GetWaveTime() const { return WaveTime; }

	UFUNCTION(BlueprintPure, Category = "Wave")
	EWaveGameFlowState GetGameFlowState() const { return GameFlowState; }

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	float WaveTime = 30.0f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Wave")
	int32 Score = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Wave")
	float RemainingTime = 30.0f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Wave")
	int32 CurrentWaveIndex = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Wave")
	EWaveGameFlowState GameFlowState = EWaveGameFlowState::BeforeStart;
};
