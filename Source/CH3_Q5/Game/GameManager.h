#pragma once

#include "CoreMinimal.h"
#include "CH3_Q5/Game/GameFinishResult.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "GameManager.generated.h"

class AGameStateWave;
class UHealthComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameManagerEventSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameFinishedSignature, EGameFinishResult, FinishResult);

UCLASS()
class CH3_Q5_API AGameManager : public AActor
{
	GENERATED_BODY()

public:
	AGameManager();

	UPROPERTY(BlueprintAssignable, Category = "Game")
	FOnGameManagerEventSignature OnGameStarted;

	UPROPERTY(BlueprintAssignable, Category = "Game")
	FOnGameFinishedSignature OnGameFinished;

	UFUNCTION(BlueprintCallable, Category = "Game")
	void StartGame();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void FinishGame(EGameFinishResult FinishResult = EGameFinishResult::Cleared);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
	int32 FirstWaveIndex = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
	float TimerTickInterval = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game", meta = (DeprecatedProperty, DeprecationMessage = "Start the game manually from Blueprint."))
	bool bAutoStartOnBeginPlay = false;

	UFUNCTION(BlueprintImplementableEvent, Category = "Game", meta = (DisplayName = "On Game Started"))
	void ReceiveGameStarted();

	UFUNCTION(BlueprintImplementableEvent, Category = "Game", meta = (DisplayName = "On Game Finished"))
	void ReceiveGameFinished(EGameFinishResult FinishResult);

private:
	FTimerHandle GameTimerHandle;
	TWeakObjectPtr<UHealthComponent> BoundPlayerHealthComponent;

	AGameStateWave* GetWaveGameState() const;
	void StartGameTimer();
	void StopGameTimer();
	void HandleGameTimerTick();
	void BindPlayerHealthComponent();
	void UnbindPlayerHealthComponent();

	UFUNCTION()
	void HandlePlayerHealthDepleted();
};
