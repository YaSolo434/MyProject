// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFirstCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Sword.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Grass.h"
#include "DamageableInterface.h"
#include "HealthBar.h"
#include "StaminaBar.h"
#include "MyFirstHUD.h"
#include "InventoryComponent.h"
#include "Pickup.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Sight.h"

// Sets default values
AMyFirstCharacter::AMyFirstCharacter() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//creates cameraboom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetCapsuleComponent());

	//creates camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	Camera->SetupAttachment(CameraBoom);
	Camera->FieldOfView = 120.f;

	//set movement stuff
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->JumpZVelocity = 800.f;
	GetCharacterMovement()->AirControl = 0.5f;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
	
	//initialize inventory
	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));
	PlayerInventory->SetSlotsCapacity(20);
	PlayerInventory->SetWeightCapacity(60);

	//set interaction trace vars
	InteractionCheckFrequency = 0.1f;
	InteractionCheckDistance = 225.0f;
	BaseEyeHeight = 78.f;
	
	//set health comp
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->CurrentHealth = 10.f;
	HealthComponent->MaxHealth = 10.f;
	
}

// Called when the game starts or when spawned
void AMyFirstCharacter::BeginPlay() {
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (PlayerController) {
			UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
			if (Subsystem && DefaultMappingContext) {
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
	
	SpawnSword();
	
	HUD = Cast<AMyFirstHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (HUD)
	{
		HealthBarWidget = HUD->GetHealthBarWidget();
		StaminaBarWidget = HUD->GetStaminaBarWidget();
	}

	//limit the camera
	APlayerCameraManager* const PlayerCameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	PlayerCameraManager->ViewPitchMin = -50.f;
	PlayerCameraManager->ViewPitchMax = 20.f;
	
	HealthComponent->OnDeath.AddDynamic(this, &AMyFirstCharacter::Die);
	HealthComponent->OnDamaged.AddDynamic(this, &AMyFirstCharacter::PlayHitAnim);
	HealthComponent->OnHealthChanged.AddDynamic(this, &AMyFirstCharacter::UpdateHealthBar);
}

// Called every frame
void AMyFirstCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// dash cooldown
	DashCooldown(DeltaTime);

	// making movement more smooth
	FVector Velocity = GetVelocity();
	FVector HorizontalVelocity = FVector(Velocity.X, Velocity.Y, 0.f);

	bool bIsMoving = HorizontalVelocity.SizeSquared() > KINDA_SMALL_NUMBER;
	GetCharacterMovement()->bOrientRotationToMovement = bIsMoving;
	
	SprintAdj(DeltaTime);
	
	//check trace line shooting frequency
	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency) {
		PerformInteractionCheck();
	}
}

//MoveForward
void AMyFirstCharacter::MoveForward(const FInputActionValue& Value) {
	if (!IsLanding) {
		IsMoving = true;

		float AxisValue = Value.Get<float>();
		if (Controller && AxisValue != 0.f)
		{
			const FRotator ControlRotation = Controller->GetControlRotation();
			const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, AxisValue);
		}
	}

}
void AMyFirstCharacter::StopMoveForward(const FInputActionValue& Value) {
	IsMoving = false;
	CachedMoveInput.X = 0.f;
}
//MoveRight
void AMyFirstCharacter::MoveRight(const FInputActionValue& Value) {	
	if (!IsLanding) {
		float AxisValue = -Value.Get<float>();
		if (Controller && AxisValue != 0.f) {
			const FRotator CameraRotation = Controller->GetControlRotation();
			const FRotator YawCameraRotation = FRotator(0.f, CameraRotation.Yaw, 0.f);
			
			FVector CameraRotationVector = FRotationMatrix(YawCameraRotation).GetUnitAxis(EAxis::Y);

			AddMovementInput(-CameraRotationVector, AxisValue);
		}
		
	}
}
void AMyFirstCharacter::StopMoveRight(const FInputActionValue& Value) {
	CachedMoveInput.Y = 0.f;
}

void AMyFirstCharacter::StartSprint(const FInputActionValue& Value) {
	if (!IsLanding && CanSprint && IsMoving) {
		IsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

	}
}

void AMyFirstCharacter::StopSprint(const FInputActionValue& Value) {
	IsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}

void AMyFirstCharacter::SprintAdj(float DeltaTime) 
{
	if (IsSprinting && CanSprint) 
	{
		CurSprintTime -= DeltaTime;
		RegenTimer = 0.f;
		if (CurSprintTime <= 0.f) {
			CurSprintTime = 0.f;
			CanSprint = false;
			GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
		}
		UpdateStaminaBarWidget();
	}
	else {
		RegenTimer += DeltaTime;
		if (RegenTimer >= DelayRegenTime) {
			CurSprintTime += DeltaTime;
			CanSprint = true;

			if (CurSprintTime >= MaxSprintTime) {
				CurSprintTime = MaxSprintTime;
				RegenTimer = 0.f;
			}
		}
		UpdateStaminaBarWidget();
	}
	UpdateStaminaBarWidget();
}

void AMyFirstCharacter::Jump() {
	if (!IsLanding) {
		if (JumpCount < MaxJumpCount) {
			if (JumpCount == 0) {
				CanSprint = false;
				IsJumping = true;
				Super::Jump();
			}
			else {
				FVector JumpVelocity = FVector(0.f, 0.f, GetCharacterMovement()->JumpZVelocity);
				LaunchCharacter(JumpVelocity, false, true);
			}
			JumpCount++;
		}
	}

}

void AMyFirstCharacter::Landed(const FHitResult& Hit) {
	Super::Landed(Hit);
	IsJumping = false;
	JumpCount = 0;

	IsLanding = true;
	GetWorldTimerManager().SetTimer(
		LandSpeedTimerHandle,
		this,
		&AMyFirstCharacter::RestoreWalkSpeed,
		1.2f,
		false
	);
}

void AMyFirstCharacter::RestoreWalkSpeed() {
	IsLanding = false;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}

void AMyFirstCharacter::Dash(float ForwardValue, float RightValue) {
	if (CanUseDash) {
		if (!IsLanding) {
		FVector Forward = GetActorForwardVector();
		FVector Right = GetActorRightVector();

		FVector DashDirection = (Forward * ForwardValue) + (Right * RightValue);
			if (!DashDirection.IsNearlyZero()) {
				DashDirection.Normalize();
				FVector DashVelocity = DashDirection * DashSpeed;
				LaunchCharacter(DashVelocity, true, false);
			}
			else {
				FVector DashVelocity = GetActorForwardVector() * DashSpeed;
				LaunchCharacter(DashVelocity, true, false);
			}
			CanUseDash = false;
		}
	}	
}

void AMyFirstCharacter::DashCooldown(float DeltaTime) {
	if (!CanUseDash) {
		CurTime += DeltaTime;
		if (CurTime >= BreakTime) {
			CanUseDash = true;
			UGameplayStatics::PlaySoundAtLocation(this, DashSound, GetActorLocation());
			CurTime = 0.f;
		}
	}
}

void AMyFirstCharacter::DashInput() {
	Dash(CachedMoveInput.X, CachedMoveInput.Y);
}


void AMyFirstCharacter::AddCoin(int Amount) {
	TotalCoin += Amount;
}

void AMyFirstCharacter::TakeXp() {
	AddCoin(2);
}

void AMyFirstCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookInput = Value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void AMyFirstCharacter::SpawnSword() {
	if (SwordClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		EquippedSword = GetWorld()->SpawnActor<ASword>(SwordClass, SpawnParams);
		if (EquippedSword)
		{
			// Attach to mesh socket
			EquippedSword->AttachToComponent(
				GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				FName("R_HandSocket")
			);
		}
	}
}

void AMyFirstCharacter::UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetHealthProgressPrecent(CurrentHealth / MaxHealth);
	}
}

void AMyFirstCharacter::PlayHitAnim()
{
	if (!bIsDead)
	{
		//get Anim Instance
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		//play anim
		AnimInstance->Montage_Play(HitReactMontage);
	}
}

void AMyFirstCharacter::Die()
{
	if (bIsDead) return;
	bIsDead = true;
	
	// Disable movement
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	
	// Disable visibility signal
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	
	if (UAnimInstance* AnimIns = GetMesh()->GetAnimInstance())
	{
		AnimIns->StopAllMontages(0.1f);
	}
	// Disable collision to prevent unintended behavior
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	//play death anim
	GetMesh()->PlayAnimation(DeathAnim, false);
	
	StaminaBarWidget->SetVisibility(ESlateVisibility::Collapsed);
	HealthBarWidget->SetVisibility(ESlateVisibility::Collapsed);
	
	SetLifeSpan(5.f);
	EquippedSword->SetLifeSpan(5.f);
	
	EquippedSword = nullptr;
}

USoundBase* AMyFirstCharacter::GetRandomWalkingSound() const {
	if (WalkingSounds.Num() > 0) {
		int32 const Index = FMath::RandRange(0, WalkingSounds.Num() - 1);
		
		return WalkingSounds[Index];
	}
	return nullptr;
}

void AMyFirstCharacter::UpdateStaminaBarWidget() const {
	if (HUD)
	{
		HUD->UpdateStaminaBar(CurSprintTime, MaxSprintTime);
	}
}

void AMyFirstCharacter::Attack(const FInputActionValue& Value) {
	if (AttackMontage) {
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && !IsAttacking) {
			AnimInstance->Montage_Play(AttackMontage);
			IsAttacking = true;
			IsLanding = true;
			GetWorldTimerManager().SetTimer(
				LandSpeedTimerHandle,
				this,
				&AMyFirstCharacter::RestoreWalkSpeed,
				1.4f,
				false
			);
		}
	}
}

void AMyFirstCharacter::LineTrace() {
	//Get SwordMesh
	UStaticMeshComponent* SwordMesh = EquippedSword->SwordStaticMesh;

	//Get socket locations
	FVector StartLocation = SwordMesh->GetSocketLocation(FName("Start"));
	FVector EndLocation = SwordMesh->GetSocketLocation(FName("End"));
	
	//Setup line trace
	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	//Linetrace
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams);
	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 1.0f, 0, 2.0f);
	if (!HitResult.GetActor()) { return; }
	
	//Get HitActor that gets hit by sword
	if (HitResult.bBlockingHit) {
		UE_LOG(LogTemp, Warning, TEXT("Hit actor: %s"), *HitResult.GetActor()->GetName());
		
		//Get actor that was hit
		AActor* ActorHit = HitResult.GetActor();
		
		//Apply damage if actor implements damageable interface
		if (ActorHit && ActorHit->GetClass()->ImplementsInterface(UDamageableInterface::StaticClass())) 
		{
			if (IDamageableInterface* Damageable = Cast<IDamageableInterface>(ActorHit))
			{
				if (Damage > 0.f && Damageable->IsDamageable())
				{
					Damageable->ApplyDamage(ActorHit, Damage);
				}
			}
		}
		
		//Play sound if actor is grass
		if (ActorHit->IsA(AGrass::StaticClass())) 
		{
			if (SwordHitSound && !HasPlayedHitSound) 
			{
				UGameplayStatics::PlaySoundAtLocation(ActorHit, SwordHitSound, HitResult.Location);

				HasPlayedHitSound = true;
			}
		}
	}
}



void AMyFirstCharacter::PerformInteractionCheck() {
	// get world time second 
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	// create trace start and end vector
	FVector TraceStart{GetPawnViewLocation()};
	FVector TraceEnd{ TraceStart + (GetViewRotation().Vector()) * InteractionCheckDistance };

	//setup line trace
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	FHitResult TraceHit;

	float LookDirection = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector());

	if (LookDirection > 0) {

		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);
		//LineTrace
		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams)) {
			if (!TraceHit.GetActor())
			{
				NoInteractableFound();
				return;
			}
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass())) {

				if (TraceHit.GetActor() != InteractionData.CurrentInteractable) {
					FoundInteractable(TraceHit.GetActor());
					return;
				}

				if (TraceHit.GetActor() == InteractionData.CurrentInteractable) {
					return;
				}
			}
		}
		NoInteractableFound();
	}
}

void AMyFirstCharacter::FoundInteractable(AActor* NewInteractable) {

	if (IsInteracting()) {
		EndInteract();
	}
	if (InteractionData.CurrentInteractable) {
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}
	
	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = InteractionData.CurrentInteractable;

	HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);

	TargetInteractable->BeginFocus();
}

void AMyFirstCharacter::NoInteractableFound() {

	if (IsInteracting()) {
		GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	}

	if (InteractionData.CurrentInteractable) {
		if (IsValid(TargetInteractable.GetObject())) {
			TargetInteractable->EndFocus();
		}
	}

	HUD->HideInteractionWidget();

	InteractionData.CurrentInteractable = nullptr;
	TargetInteractable = nullptr;
}

void AMyFirstCharacter::BeginInteract() {

	// verify nothing has chagned since tick func
	PerformInteractionCheck();

	if (InteractionData.CurrentInteractable) {
		if (IsValid(TargetInteractable.GetObject())) {
			TargetInteractable->BeginInteract();

			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f)) {
				Interact();
			}
			else {
				GetWorldTimerManager().SetTimer(TimerHandle_Interaction,
					this,
					&AMyFirstCharacter::Interact,
					TargetInteractable->InteractableData.InteractionDuration,
					false
				);
			}
		}	
	}
}

void AMyFirstCharacter::Interact() {
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject())) {
		TargetInteractable->Interact(this);
		UGameplayStatics::PlaySoundAtLocation(this, DashSound, GetActorLocation());
	}
}

void AMyFirstCharacter::EndInteract() {
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject())) {
		TargetInteractable->EndInteract();
	}
}

void AMyFirstCharacter::DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	if (PlayerInventory->FindMatchingItem(ItemToDrop))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		const FVector SpawnLocation{ GetActorLocation() + (GetActorForwardVector() * 50.0f) };
		const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

		const int32 RemovedQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);

		APickup* Pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), SpawnTransform, SpawnParams);

		Pickup->InitializeDrop(ItemToDrop, RemovedQuantity);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item to drop was somehow failed!"));
	}
}

void AMyFirstCharacter::ApplyDamage(AActor* DamagedActor, float DamageAmount)
{
	if (DamagedActor && DamageAmount > 0.f)
	{
		IDamageableInterface* Damageable = Cast<IDamageableInterface>(DamagedActor);
		
		if (Damageable && Damageable->IsDamageable())
		{
			Damageable->ReceiveDamage(this, DamageAmount);
		}
	}
}

void AMyFirstCharacter::ReceiveDamage(AActor* DamageCauser, float DamageAmount)
{
	if (HealthComponent && DamageCauser && DamageAmount > 0.f)
	{
		HealthComponent->TakeDamage(DamageAmount);
	}
}

bool AMyFirstCharacter::IsDamageable() const
{
	return true;
}

bool AMyFirstCharacter::IsActorDead(AActor* Actor) const
{
	return bIsDead;
}

UHealthComponent* AMyFirstCharacter::GetHealthComponent() const
{
	return HealthComponent;
}

void AMyFirstCharacter::UpdateInteractionWidget() const {
	if (IsValid(TargetInteractable.GetObject())) {
		HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}
}

void AMyFirstCharacter::ToggleMenu() {
	HUD->ToggleMenu();
}

void AMyFirstCharacter::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));

	if (StimulusSource)
	{
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}

// Called to bind functionality to input
void AMyFirstCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		//add input for forward action
		Input->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &AMyFirstCharacter::MoveForward);
		Input->BindAction(MoveForwardAction, ETriggerEvent::Completed, this, &AMyFirstCharacter::StopMoveForward);

		//add input for right action
		Input->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &AMyFirstCharacter::MoveRight);
		Input->BindAction(MoveRightAction, ETriggerEvent::Completed, this, &AMyFirstCharacter::StopMoveRight);

		Input->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyFirstCharacter::Jump);
		Input->BindAction(DashAction, ETriggerEvent::Started, this, &AMyFirstCharacter::DashInput);

		Input->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AMyFirstCharacter::StartSprint);
		Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMyFirstCharacter::StopSprint);

		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyFirstCharacter::Look);

		Input->BindAction(AttackAction, ETriggerEvent::Started, this, &AMyFirstCharacter::Attack);

		Input->BindAction(InteractAction, ETriggerEvent::Started, this, &AMyFirstCharacter::BeginInteract);
		Input->BindAction(InteractAction, ETriggerEvent::Completed, this, &AMyFirstCharacter::EndInteract);
		
		Input->BindAction(ToggleMenuAction, ETriggerEvent::Started, this, &AMyFirstCharacter::ToggleMenu);
	}
}
