// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageableInterface.h"

// Add default functionality here for any IIDamageableInterface functions that are not pure virtual.

void IDamageableInterface::ApplyDamage(AActor* DamagedActor, float DamageAmount) {}

void IDamageableInterface::ReceiveDamage(AActor* DamageCauser, float DamageAmount) {}

bool IDamageableInterface::IsDamageable() const
{
	return false;
}

bool IDamageableInterface::IsActorDead(AActor* Actor) const
{
	return false;
}

UHealthComponent* IDamageableInterface::GetHealthComponent() const
{
	return nullptr;
}
