// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveObjects.h"
#include "MyActor.h"

// Sets default values for this component's properties
UMoveObjects::UMoveObjects()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMoveObjects::BeginPlay()
{
	Super::BeginPlay();

	StartPosition = GetRelativeLocation();

	FVector NormalizedOffset = MoveOffset;
	NormalizedOffset.Normalize();

	MaxDistance = MoveOffset.Length();

	MyActor = Cast<AMyActor>(GetOwner());

}
// Called every frame
void UMoveObjects::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (MyActor && MyActor->ShouldMove) {

		CurrentDistance += DeltaTime * speed * Direction;

		if (CurrentDistance >= MaxDistance or CurrentDistance <= 0.0f) {
		Direction *= -1;
		}
	}

	SetRelativeLocation(StartPosition + MoveOffset * CurrentDistance);
}