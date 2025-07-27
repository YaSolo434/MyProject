// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MyActor.h"
#include "MoveObjects.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UMoveObjects : public USceneComponent
{
	GENERATED_BODY(UMoveObjects)

public:	
	// Sets default values for this component's properties
	UMoveObjects();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = "Movement")
	float speed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector MoveOffset;

	FVector StartPosition;

	UPROPERTY()
	AMyActor* MyActor;

	float MaxDistance;
	float CurrentDistance;

	void ChangeDirection(int Direction);
	int Direction = 1;

};
