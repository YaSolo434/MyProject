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
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Sword.h"

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
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	float MovementSpeed = 330.f;
	float RotationSpeed = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	float SprintSpeed = 800.f;
	bool CanSprint = true;
	float CurSprintTime = 0.f;

	const int MaxJumpCount = 2;
	int JumpCount = 0;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "ture"))
	bool IsJumping = false;

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

	FVector2D CachedMoveInput;

	FTimerHandle LandSpeedTimerHandle;
	bool IsLanding = false;
	UFUNCTION()
	void RestoreWalkSpeed();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveForwardAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveRightAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* DashAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* SprintAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookAction;

	void MoveForward(const FInputActionValue& Value);
	void StopMoveForward(const FInputActionValue& Value);

	void MoveRight(const FInputActionValue& Value);
	void StopMoveRight(const FInputActionValue& Value);

	void StartSprint(const FInputActionValue& Value);
	void StopSprint(const FInputActionValue& Value);

	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;

	void Dash(float ForwardValue, float RightValue);
	void DashInput();

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class ASword> SwordClass;

	UPROPERTY()
	ASword* EquippedSword;
};
