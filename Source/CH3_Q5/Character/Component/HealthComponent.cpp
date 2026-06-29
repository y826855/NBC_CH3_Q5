#include "CH3_Q5/Character/Component/HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	ResetHealth();
}

void UHealthComponent::ApplyDamage(float DamageAmount)
{
	if (DamageAmount <= 0.0f)
	{
		return;
	}

	SetCurrentHealth(CurrentHealth - DamageAmount);
}

void UHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.0f)
	{
		return;
	}

	SetCurrentHealth(CurrentHealth + HealAmount);
}

void UHealthComponent::ResetHealth()
{
	SetCurrentHealth(MaxHealth);
}

void UHealthComponent::SetHealth(float NewHealth)
{
	SetCurrentHealth(NewHealth);
}

float UHealthComponent::GetHealthPercent() const
{
	if (MaxHealth <= 0.0f)
	{
		return 0.0f;
	}

	return CurrentHealth / MaxHealth;
}

void UHealthComponent::SetCurrentHealth(float NewHealth)
{
	const float PreviousHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);

	if (PreviousHealth > 0.0f && CurrentHealth <= 0.0f)
	{
		OnHealthDepleted.Broadcast();
	}
}
