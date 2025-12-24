// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	CanTakeDamage = true;
}

void UHealthComponent::TakeDamage(int Damage) {
	if (CanTakeDamage) {
		CurrentHealth -= Damage;
		CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);

		OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
		OnDamaged.Broadcast();

		CanTakeDamage = false;

		//delay
		FTimerHandle InvincibilityDelay;
		GetWorld()->GetTimerManager().SetTimer(InvincibilityDelay, this, &UHealthComponent::AllowTakeDamage, 0.5f, false);

		//Die Func
		if (CurrentHealth <= 0) {
			OnDeath.Broadcast();
		}
	}
}

void UHealthComponent::AllowTakeDamage() {

	CanTakeDamage = true;
}

