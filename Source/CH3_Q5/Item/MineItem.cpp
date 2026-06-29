// Fill out your copyright notice in the Description page of Project Settings.


#include "MineItem.h"

#include "CH3_Q5.h"
#include "CH3_Q5/Character/Component/HealthComponent.h"

// Sets default values
AMineItem::AMineItem()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMineItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (!Activator)
	{
		return;
	}

	UHealthComponent* HealthComponent = Activator->FindComponentByClass<UHealthComponent>();
	if (!HealthComponent)
	{
		return;
	}

	HealthComponent->ApplyDamage(DamageAmount);
}

