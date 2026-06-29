// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameTimer.generated.h"

class AGameStateWave;
class UTextBlock;

UCLASS()
class CH3_Q5_API UGameTimer : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Timer")
	void RefreshFromGameState();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> RemainingTimeText;

private:
	UPROPERTY()
	TObjectPtr<AGameStateWave> CachedGameState;

	UFUNCTION()
	void HandleRemainingTimeChanged(float NewRemainingTime);

	void BindToGameState();
	void UnbindFromGameState();
	void SetRemainingTimeText(float NewRemainingTime);
};
