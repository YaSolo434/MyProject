// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFirstCharacter.h"

// Sets default values
AMyFirstCharacter::AMyFirstCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//creating camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetCapsuleComponent());
	
	//creating mesh
	CharacterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CharacterMesh"));
	CharacterMesh->SetupAttachment(GetCapsuleComponent());
	Camera->FieldOfView = 120.f;
}

void AMyFirstCharacter::MoveForward(float Value) {
	AddMovementInput(GetActorForwardVector(), Value * MovementSpeed);
}

void AMyFirstCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value * MovementSpeed);
	
}

void AMyFirstCharacter::Jump() {
	GetCharacterMovement()->JumpZVelocity = 900.f;

	Super::Jump();
}

void AMyFirstCharacter::JumpPressed() {
	Jump();
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
void AMyFirstCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyFirstCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyFirstCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveL_R"), this, &AMyFirstCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("MoveF_B"), this, &AMyFirstCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Rotate"), this, &AMyFirstCharacter::Rotate);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyFirstCharacter::JumpPressed);

}

