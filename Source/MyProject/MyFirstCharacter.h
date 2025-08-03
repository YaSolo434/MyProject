// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TakingXp.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Player Settings")
	float MovementSpeed = 800.f;
	float RotationSpeed = 1.0f;

	const int MaxJumpCount = 2;
	int JumpCount = 0;

	int TotalCoin;

	UPROPERTY(EditAnywhere)
	float DashSpeed = 600.f;

	bool CanUseDash = true;
	float BreakTime = 5.0f;
	float CurTime = 0.f;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* DashSound;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	void MoveForward(float Value);
	void MoveRight(float Value);
	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;

	void Dash(float ForwardValue, float RightValue);
	void DashInput();
};
