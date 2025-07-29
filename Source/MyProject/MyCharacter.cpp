// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "TakingXp.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// adding camera and cameramash
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CameraMesh"));

	Camera->SetupAttachment(CameraMesh);
	Camera->AddRelativeLocation(FVector(-200.0f, 0.0f, 150.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));

	CameraMesh->SetStaticMesh(CubeAsset.Object);

	// camera adjs
	Camera->FieldOfView = 120.f;
}

void AMyCharacter::Move(float ForwardValue, float RightValue, float DeltaTime) {
	FVector Direction = FVector(ForwardValue, RightValue, 0.f);

	if (!Direction.IsNearlyZero()) {
		Direction.Normalize();
		FVector NewLocation = GetActorLocation();
		NewLocation += DeltaTime * Direction * mov_adj;
		SetActorLocation(NewLocation);
	}
}	

void AMyCharacter::MoveForward(float Value)
{
	CurrentForwardValue = Value;
}	

void AMyCharacter::MoveRight(float Value)
{
	CurrentRightValue = Value;
}

void AMyCharacter::AddCoin(int Amount) {
	TotalCoin += Amount;
}

void AMyCharacter::TakeXp() {
	AddCoin(2);
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AMyCharacter::Move(CurrentForwardValue, CurrentRightValue, DeltaTime);
	
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveL_R"), this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("MoveF_B"), this, &AMyCharacter::MoveForward);
}