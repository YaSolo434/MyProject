// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "HealthComponent.h"
#include "MyFirstCharacter.h"

#include "Grass.generated.h"

UCLASS()
class MYPROJECT_API AGrass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* GrassMesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* GrassHitbox;

	UPROPERTY(EditAnywhere)
	UHealthComponent* HealthComp;

	float Scale = 1.f;

	UPROPERTY(EditAnywhere, Category = "Scale")
	float ScaleChange = 0.3f;
	
protected:

	UFUNCTION()
	void GettinDamage();

	UFUNCTION()
	void Kill();
};

