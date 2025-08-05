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

void AMyFirstCharacter::MoveForward(const FInputActionValue& Value) {
	CachedMoveInput.X = Value.Get<float>();
	AddMovementInput(GetActorForwardVector(), CachedMoveInput.X);

	AMyFirstCharacter::UpdateLastMoveDirection();


}
void AMyFirstCharacter::StopMoveForward(const FInputActionValue& Value) {
	CachedMoveInput.X = 0.f;
}

void AMyFirstCharacter::MoveRight(const FInputActionValue& Value) {	
	CachedMoveInput.Y = -Value.Get<float>();
	AddMovementInput(GetActorRightVector(), CachedMoveInput.Y);

	AMyFirstCharacter::UpdateLastMoveDirection();
}
void AMyFirstCharacter::StopMoveRight(const FInputActionValue& Value) {
	CachedMoveInput.Y = 0.f;
}

void AMyFirstCharacter::Jump() {
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


void AMyFirstCharacter::Landed(const FHitResult& Hit) {
	Super::Landed(Hit);
	IsJumping = false;
	JumpCount = 0;
}

void AMyFirstCharacter::Dash(float ForwardValue, float RightValue) {
	if (CanUseDash) {
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


void AMyFirstCharacter::DashInput() {
	Dash(CachedMoveInput.X, CachedMoveInput.Y);
}

void AMyFirstCharacter::AddCoin(int Amount) {
	TotalCoin += Amount;
}

void AMyFirstCharacter::TakeXp() {
	AddCoin(2);
}

void AMyFirstCharacter::UpdateLastMoveDirection() {
	if (!FMath::IsNearlyZero(CachedMoveInput.X) || !FMath::IsNearlyZero(CachedMoveInput.Y)) {
		LastNonZeroMoveInput = CachedMoveInput;
	}
}

void AMyFirstCharacter::UpdateMeshRotation() {
	if (LastNonZeroMoveInput.Y > 0.1f) {
		TargetMeshRotation = FRotator(0.f, 0.f, 0.f);
	}
	else if (LastNonZeroMoveInput.Y < -0.1f) {
		TargetMeshRotation = FRotator(0.f, 180.f, 0.f);
	}
	else if (LastNonZeroMoveInput.X > 0.1f) {
		TargetMeshRotation = FRotator(0.f, -90.f, 0.f);
	}
	else if (LastNonZeroMoveInput.X < -0.1f) {
		TargetMeshRotation = FRotator(0.f, 90.f, 0.f);
	}

	if (LastNonZeroMoveInput.X > 0.1f && LastNonZeroMoveInput.Y > 0.1f) {
		TargetMeshRotation = FRotator(0.f, -45.f, 0.f);
	}
	else if (LastNonZeroMoveInput.X > 0.1f && LastNonZeroMoveInput.Y < -0.1f) {
		TargetMeshRotation = FRotator(0.f, -135.f, 0.f);
	}
	else if (LastNonZeroMoveInput.X < -0.1f && LastNonZeroMoveInput.Y > 0.1f) {
		TargetMeshRotation = FRotator(0.f, 45.f, 0.f);
	}
	else if (LastNonZeroMoveInput.X < -0.1f && LastNonZeroMoveInput.Y < -0.1f) {
		TargetMeshRotation = FRotator(0.f, 135.f, 0.f);
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
	AMyFirstCharacter::UpdateMeshRotation();

	FRotator CurrentRotation = GetMesh()->GetRelativeRotation();
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetMeshRotation, DeltaTime, MeshRotationSpeed);
	GetMesh()->SetRelativeRotation(NewRotation);
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
	}
}