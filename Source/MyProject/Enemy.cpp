// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthBar.h"
#include "Components/WidgetComponent.h"

#include "Enemy.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(RootComponent);
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::World);
	HealthBarWidget->SetDrawSize(FVector2D(100.f, 10.f));
	HealthBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 120.f));


	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	SwordMesh->SetupAttachment(GetMesh(), FName("R_HandSocket"));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	HealthComp->OnDeath.AddDynamic(this, &AEnemy::Die);
	HealthComp->OnDamaged.AddDynamic(this, &AEnemy::HitAnimation);
	HealthComp->OnHealthChanged.AddDynamic(this, &AEnemy::UpdateHealthBar);
}

void AEnemy::Die() {

	//disabling movement and collision to prevent hapenin shitty things
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//playing death anim
	GetMesh()->PlayAnimation(DeathAnim, false);
}

void AEnemy::HitAnimation() {
	
	//getting Anim Insatnce
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	//playing anim
	AnimInstance->Montage_Play(HitReactMontage);
}

void AEnemy::UpdateHealthBar(float CurrentHealth, float MaxHealth) {
	if (UUserWidget* Widget = HealthBarWidget->GetUserWidgetObject()) {
		if (UHealthBar* HealthBar = Cast<UHealthBar>(Widget)) {
			HealthBar->SetHealthPrecent(CurrentHealth / MaxHealth);
		}
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}