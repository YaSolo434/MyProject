// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "MyCharacter.h"
#include "MoveObjects.h"
// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// creating StartOverlap event
void AMyActor::OnHitboxOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult){

    if (OtherActor && OtherActor != this)
    {
        // Check if the overlapping actor is exactly MyCharacter (or subclass)
        if (OtherActor->IsA(AMyCharacter::StaticClass()))
        {
            ShouldMove = true;
            ShouldGoBack = false;
        }
    }
  
}

void AMyActor::OnHitboxEndOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex) {
    
    if (OtherActor->IsA(AMyCharacter::StaticClass())) {

        ShouldGoBack = true;
    }
}
// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	// getting the hitbox from bp class
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	Hitbox = Cast<UBoxComponent>(GetDefaultSubobjectByName(TEXT("Hitbox")));
    //adding listener for BeginOverLap
    Hitbox->OnComponentBeginOverlap.AddDynamic(this, &AMyActor::OnHitboxOverlap);

    Hitbox->OnComponentEndOverlap.AddDynamic(this, &AMyActor::OnHitboxEndOverlap);
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

