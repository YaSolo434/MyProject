// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFirstCharacter.h"

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

	//adding dash sound
	static ConstructorHelpers::FObjectFinder<USoundBase> SoundObj(TEXT("/Game/Sounds/coin.coin"));
	DashSound = SoundObj.Object;
}
//MoveForward
void AMyFirstCharacter::MoveForward(const FInputActionValue& Value) {
	if (!IsLanding) {
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
	CachedMoveInput.X = 0.f;
}
//MoveRight
void AMyFirstCharacter::MoveRight(const FInputActionValue& Value) {	
	if (!IsLanding) {
		float AxisValue = Value.Get<float>();
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
	if (!IsLanding && CanSprint) {
		IsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

	}
}

void AMyFirstCharacter::StopSprint(const FInputActionValue& Value) {
	IsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}

void AMyFirstCharacter::SprintAdj(float DeltaTime) {
	if (IsSprinting && CanSprint) {
		CurSprintTime -= DeltaTime;
		RegenTimer = 0.f;
		if (CurSprintTime <= 0.f) {
			CurSprintTime = 0.f;
			CanSprint = false;
			GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
		}
	}
	else {
		RegenTimer += DeltaTime;
		if (RegenTimer >= DelayRegenTime) {
			CurSprintTime += DeltaTime;
			if (CurSprintTime >= MaxSprintTime) {
				CurSprintTime = MaxSprintTime;
				RegenTimer = 0.f;
				CanSprint = true;
			}
		}
	}
}

void AMyFirstCharacter::Jump() {
	if (!IsLanding) {
		if (JumpCount < MaxJumpCount) {
			if (JumpCount == 0) {
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
	
	//Setup linetrace
	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	//Linetrace
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams);
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 1.0f, 0, 2.0f);

	//Get HitActor that gets hit by sword
	if (HitResult.bBlockingHit) {
		UE_LOG(LogTemp, Warning, TEXT("Hit actor: %s"), *HitResult.GetActor()->GetName());
		AActor* ActorHit = HitResult.GetActor();
		UHealthComponent* EnemyHit = ActorHit->FindComponentByClass<UHealthComponent>();

		if (EnemyHit) {
			UE_LOG(LogTemp, Warning, TEXT("Health component found! Applying damage."));
			EnemyHit->TakeDamage(Damage);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("No health component found on hit actor."));
		}
	}
}


// Called when the game starts or when spawned
void AMyFirstCharacter::BeginPlay() {
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController) {
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem && DefaultMappingContext) {
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	SpawnSword();
}

// Called every frame
void AMyFirstCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (!CanUseDash) {
		CurTime += DeltaTime;
		if (CurTime >= BreakTime) {
			CanUseDash = true;
			UGameplayStatics::PlaySoundAtLocation(this, DashSound, GetActorLocation());
			CurTime = 0.f;
		}
	}
	FVector Velocity = GetVelocity();
	FVector HorizontalVelocity = FVector(Velocity.X, Velocity.Y, 0.f);

	bool bIsMoving = HorizontalVelocity.SizeSquared() > KINDA_SMALL_NUMBER;

	GetCharacterMovement()->bOrientRotationToMovement = bIsMoving;

	SprintAdj(DeltaTime);
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
	}
}