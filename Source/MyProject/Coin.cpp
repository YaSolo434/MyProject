// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "MyCharacter.h"

// Sets default values
ACoin::ACoin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	CoinHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("CoinHitbox"));
	CoinHitbox->SetupAttachment(StaticMesh);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
    StaticMesh->SetStaticMesh(CubeAsset.Object);

    StaticMesh->SetSimulatePhysics(true);
	
}
void ACoin::OnHitboxOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult) {
    if (OtherActor->IsA(AMyCharacter::StaticClass()))
    {
        if (!IsJumped) {
        StaticMesh->AddImpulse(FVector(0.f, 0.f, Velocity), NAME_None, true);
        IsJumped = true;
        
        }
    }
}

// Called when the game starts or when spawned
void ACoin::BeginPlay()
{
    Super::BeginPlay();

    CoinHitbox->OnComponentBeginOverlap.AddDynamic(this, &ACoin::OnHitboxOverlap);

}

// Called every frame
void ACoin::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (IsJumped) {
        LiveTime -= DeltaTime;
    }
    if (LiveTime <= 0.0) {
        Destroy();
    }

}

