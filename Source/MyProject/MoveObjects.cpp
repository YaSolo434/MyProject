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

	// setting start position
	StartPosition = GetRelativeLocation();

	// creating normalized vector
	FVector NormalizedOffset = MoveOffset;
	NormalizedOffset.Normalize();

	// setting max distance 
	MaxDistance = MoveOffset.Length();

	//getting MyActor from other class
	MyActor = Cast<AMyActor>(GetOwner());

}

void UMoveObjects::ChangeDirection(int NewDirection) {
	if (NewDirection >= 1) {
		Direction *= -1;
	}
}
// Called every frame
void UMoveObjects::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// movement logic
	if (MyActor && MyActor->ShouldMove) {
		CurrentDistance += DeltaTime * speed * Direction;

		if (CurrentDistance >= MaxDistance or CurrentDistance <= 0.0f) {
			Direction *= -1;
		}
		CurrentDistance = FMath::Clamp(CurrentDistance, 0.0f, MaxDistance);

		SetRelativeLocation(StartPosition + MoveOffset * CurrentDistance);

		if (MyActor->ShouldGoBack) {
			UMoveObjects::ChangeDirection(Direction);
			if (CurrentDistance == 0.0f) {
				MyActor->ShouldMove = false;
			}
		}
	}
}