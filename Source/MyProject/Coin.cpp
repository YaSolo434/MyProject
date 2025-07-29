// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
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

    static ConstructorHelpers::FObjectFinder<USoundBase> SoundObj(TEXT("/Game/Sounds/coin.coin"));
    CoinSound = SoundObj.Object;
	
}
void ACoin::OnHitboxOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult) {
    if (OtherActor->IsA(TriggerClass)) {
        if (OtherActor->GetClass()->ImplementsInterface(UTakingXp::StaticClass()))
        {
            ITakingXp* Interface = Cast<ITakingXp>(OtherActor);
            if (Interface)
            {
                Interface->TakeXp();

                UGameplayStatics::PlaySoundAtLocation(this, CoinSound, GetActorLocation());
                Destroy();
            }
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
}

