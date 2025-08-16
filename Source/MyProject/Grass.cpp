// Fill out your copyright notice in the Description page of Project Settings.


#include "Grass.h"

// Sets default values
AGrass::AGrass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

	GrassMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grass Mesh"));
	RootComponent = GrassMesh;

	GrassHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Grass Hitbox"));
	GrassHitbox->SetupAttachment(GrassMesh);

}

void AGrass::GettinDamage() {
	Scale -= ScaleChange;
	GrassMesh->SetRelativeScale3D(FVector(1.f, 1.f, Scale));

}

void AGrass::Kill() {
	Destroy();
}

// Called when the game starts or when spawned
void AGrass::BeginPlay()
{
	Super::BeginPlay();

	HealthComp->OnDamaged.AddDynamic(this, &AGrass::GettinDamage);
	HealthComp->OnDeath.AddDynamic(this, &AGrass::Kill);
}

// Called every frame
void AGrass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


