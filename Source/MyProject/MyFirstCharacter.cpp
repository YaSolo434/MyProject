// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFirstCharacter.h"

// Sets default values
AMyFirstCharacter::AMyFirstCharacter() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//creating camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->FieldOfView = 120.f;
	
	//creating mesh
	CharacterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CharacterMesh"));
	CharacterMesh->SetupAttachment(GetCapsuleComponent());

	//set movement stuff
	GetCharacterMovement()->GravityScale = 2.5f;
	GetCharacterMovement()->JumpZVelocity = 900.f;
	GetCharacterMovement()->AirControl = 0.5f;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;

	//adding dash sound
	static ConstructorHelpers::FObjectFinder<USoundBase> SoundObj(TEXT("/Game/Sounds/coin.coin"));
	DashSound = SoundObj.Object;
}

void AMyFirstCharacter::MoveForward(float Value) {
	AddMovementInput(GetActorForwardVector(), Value);
}

void AMyFirstCharacter::MoveRight(float Value) {
	AddMovementInput(GetActorRightVector(), Value);
}

void AMyFirstCharacter::Jump() {
	if (JumpCount < MaxJumpCount) {
		if (JumpCount == 0) {
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
	float ForwardValue = GetInputAxisValue("MOVEF_B");
	float RightValue = GetInputAxisValue("MOVEL_R");

	Dash(ForwardValue, RightValue);
}

void AMyFirstCharacter::Rotate(float Value) {
	AddControllerYawInput(Value);
}

void AMyFirstCharacter::AddCoin(int Amount) {
	TotalCoin += Amount;
}

void AMyFirstCharacter::TakeXp() {
	AddCoin(2);
}

// Called when the game starts or when spawned
void AMyFirstCharacter::BeginPlay() {
	Super::BeginPlay();
	
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
}

// Called to bind functionality to input
void AMyFirstCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveL_R"), this, &AMyFirstCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("MoveF_B"), this, &AMyFirstCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Rotate"), this, &AMyFirstCharacter::Rotate);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyFirstCharacter::Jump);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AMyFirstCharacter::DashInput);
}