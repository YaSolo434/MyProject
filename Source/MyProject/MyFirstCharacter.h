// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TakingXp.h"
#include "InteractionInterface.h"

#include "MyFirstCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UStaticMeshComponent;
class UCapsuleComponent;
class UCharacterMovementComponent;
class USoundBase;
class UInputMappingContext;
class UInputAction;
class UAudioComponent;
class USphereComponent;
class UWidgetComponent;
class UAIPerceptionStimuliSourceComponent;

class ASword;
class UHealthComponent;
class ATakingXp;
class AMyFirstHUD;
class UInventoryComponent;
class UStaminaBar;
class UItemBase;

USTRUCT()
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()

	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.f)
	{
	};

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
};


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

	FORCEINLINE UInventoryComponent* GetInventory() const { return PlayerInventory; };

	void UpdateInteractionWidget() const;

	void AddCoin(int Amount);

	virtual void TakeXp() override;

	UPROPERTY(BlueprintReadWrite)
	bool IsAttacking;

	UFUNCTION(BlueprintCallable)
	void LineTrace();

	void DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop);

private:

	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	float MovementSpeed = 300.f;
	float RotationSpeed = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	float SprintSpeed = 800.f;

	bool CanSprint = true;
	bool IsSprinting = false;
	bool IsMoving = false;

	float CurSprintTime = MaxSprintTime;
	float MaxSprintTime = 5.f;
	float RegenTimer = 0.f;
	float DelayRegenTime = 1.f;

	void SprintAdj(float DeltaTime);

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

	void DashCooldown(float DeltaTime);

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

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* AttackAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ToggleMenuAction;


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

	void Attack(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class ASword> SwordClass;

	UPROPERTY()
	ASword* EquippedSword;

	void SpawnSword();
	UPROPERTY(EditAnywhere, Category = "Combat")
	class UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere)
	int Damage;

	UPROPERTY(EditAnywhere, Category = "Combat|SFX")
	USoundBase* SwordHitSound;

	UPROPERTY(BlueprintReadWrite)
	bool HasPlayedHitSound = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	TArray<USoundBase*> WalkingSounds;

	UFUNCTION(BlueprintCallable, Category = "Audio")
	USoundBase* GetRandomWalkingSound() const;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UStaminaBar> StaminaBarWidgetClass;

	UPROPERTY()
	UStaminaBar* StaminaBarWidget;

	UFUNCTION()
	void UpdateStaminaBar() const;




	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	float InteractionCheckFrequency;

	float InteractionCheckDistance;

	FTimerHandle TimerHandle_Interaction;

	FInteractionData InteractionData;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	UInventoryComponent* PlayerInventory;


	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();
	void Interact();

	void ToggleMenu();

	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); };

	UPROPERTY()
	AMyFirstHUD* HUD;


	//AI
	UPROPERTY()
	class UAIPerceptionStimuliSourceComponent* StimulusSource;
	
	void SetupStimulusSource();
};
