// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageableInterface.generated.h"

class UHealthComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYPROJECT_API IDamageableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ApplyDamage(AActor* DamagedActor, float DamageAmount);
	
	virtual void ReceiveDamage(AActor* DamageCauser, float DamageAmount);
	
	virtual bool IsDamageable() const;
	
	virtual bool IsActorDead(AActor* Actor) const;
	
	virtual UHealthComponent* GetHealthComponent() const;
};
