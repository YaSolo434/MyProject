// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TakingXp.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "MyFirstCharacter.generated.h"

UCLASS()
class MYPROJECT_API AMyFirstCharacter : public ACharacter, public ITakingXp
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyFirstCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void AddCoin(int Amount);

	virtual void TakeXp() override;

private:
	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CharacterMesh;

	UPROPERTY(EditAnywhere, Category = "Player Settings")
	float MovementSpeed;
	float RotationSpeed = 1.0f;

	int TotalCoin;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Rotate(float Value);
	void JumpPressed();

	virtual void Jump() override;

};
