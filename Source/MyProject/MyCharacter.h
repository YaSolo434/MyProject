// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MyCharacter.generated.h"

UCLASS()
class MYPROJECT_API AMyCharacter : public APawn
{
	GENERATED_BODY()


public:
	// Sets default values for this pawn's properties
	AMyCharacter();


	void Move(float ForwardValue, float RightValue);
	void MoveForward(float Value);
	void MoveRight(float Value);

protected:	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)	
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CameraMesh;

	UPROPERTY(EditAnywhere, Category = "Player Settings")
	float mov_adj;
	float CurrentForwardValue = 0.0f;
	float CurrentRightValue = 0.0f;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
