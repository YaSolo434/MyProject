// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	CanTakeDamage = true;
}

void UHealthComponent::TakeDamage(int Damage) {
	if (CanTakeDamage) {
		Health -= Damage;
		OnDamaged.Broadcast();

		CanTakeDamage = false;

		//delay
		FTimerHandle InvincibilityDelay;
		GetWorld()->GetTimerManager().SetTimer(InvincibilityDelay, this, &UHealthComponent::AllowTakeDamage, 0.5f, false);

		//Die Func
		if (Health <= 0) {
			OnDeath.Broadcast();
		}
	}
}

void UHealthComponent::AllowTakeDamage() {

	CanTakeDamage = true;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


