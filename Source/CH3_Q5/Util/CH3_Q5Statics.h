#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CH3_Q5Statics.generated.h"

class AGameStateWave;
class UGameManagerSubsystem;
class UItemSpawnSubsystem;

UCLASS()
class CH3_Q5_API UCH3_Q5Statics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "CH3_Q5", meta = (WorldContext = "WorldContextObject"))
	static UItemSpawnSubsystem* GetItemSpawnSubsystem(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "CH3_Q5", meta = (WorldContext = "WorldContextObject"))
	static AGameStateWave* GetWaveGameState(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "CH3_Q5", meta = (WorldContext = "WorldContextObject"))
	static UGameManagerSubsystem* GetGameManagerSubsystem(const UObject* WorldContextObject);
};
