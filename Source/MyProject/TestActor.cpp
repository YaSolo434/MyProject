// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;


}

void ATestActor::BeginFocus() {

	Mesh->SetRenderCustomDepth(true);
}

void ATestActor::EndFocus() {

	Mesh->SetRenderCustomDepth(false);
}

void ATestActor::BeginInteract() {

	UE_LOG(LogTemp, Warning, TEXT("Calling BeginInteract override on interface test actor "));
}

void ATestActor::EndInteract() {
	UE_LOG(LogTemp, Warning, TEXT("Calling EndInteract override on interface test actor "));
}

void ATestActor::Interact(AMyFirstCharacter* PlayerCharacter) {
	UE_LOG(LogTemp, Warning, TEXT("Calling Interact override on interface test actor "));
}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();

	InteractableData = InstanceInteractableData;
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


