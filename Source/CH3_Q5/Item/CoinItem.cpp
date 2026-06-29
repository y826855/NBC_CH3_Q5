// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinItem.h"

#include "CH3_Q5/Subsystem/GameManagerSubsystem.h"
#include "CH3_Q5/Util/CH3_Q5Statics.h"


// Sets default values
ACoinItem::ACoinItem()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (UGameManagerSubsystem* GameManagerSubsystem = UCH3_Q5Statics::GetGameManagerSubsystem(this))
	{
		GameManagerSubsystem->AddScore(PointValue);
	}
}
