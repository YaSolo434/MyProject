// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TakingXp.h"
#include "MyCharacter.generated.h"

UCLASS()
class MYPROJECT_API AMyCharacter : public APawn, public ITakingXp
{
	GENERATED_BODY()


public:
	// Sets default values for this pawn's properties
	AMyCharacter();

protected:	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)	
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CameraMesh;

	UPROPERTY(EditAnywhere, Category = "Player Settings")
	float mov_adj;
	float RotationSpeed = 1.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void AddCoin(int Amount);
	virtual void TakeXp() override;
private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Rotate(float Value);

	int TotalCoin;

};
