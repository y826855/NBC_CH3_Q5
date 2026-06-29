#pragma once

#include "CoreMinimal.h"
#include "CH3_Q5/Wave/GameStateWave.h"
#include "Subsystems/WorldSubsystem.h"
#include "TimerManager.h"
#include "ItemSpawnSubsystem.generated.h"

class ASpawnVolume;

UCLASS()
class CH3_Q5_API UItemSpawnSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void RegisterSpawnVolume(ASpawnVolume* SpawnVolume);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void UnregisterSpawnVolume(ASpawnVolume* SpawnVolume);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SetSpawnInterval(float NewSpawnInterval);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnRegisteredVolumesOnce();

	UFUNCTION(BlueprintPure, Category = "Spawning")
	float GetSpawnInterval() const { return SpawnInterval; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	float SpawnInterval = 2.0f;

private:
	TArray<TWeakObjectPtr<ASpawnVolume>> SpawnVolumes;
	FTimerHandle SpawnTimerHandle;

	UPROPERTY()
	TObjectPtr<AGameStateWave> CachedGameState;

	UFUNCTION()
	void HandleGameFlowStateChanged(EWaveGameFlowState NewState);

	void BindToGameState();
	void SyncSpawnTimerWithGameState();
	void StartSpawnTimer();
	void StopSpawnTimer();
	void HandleSpawnTimerTick();
	void RemoveInvalidSpawnVolumes();
};
