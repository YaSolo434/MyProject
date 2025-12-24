// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "HealthBar.h"
#include "Components/WidgetComponent.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed = IdleSpeed;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(RootComponent);
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::World);
	HealthBarWidget->SetDrawSize(FVector2D(100.f, 10.f));
	HealthBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 120.f));


	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	SwordMesh->SetupAttachment(GetMesh(), FName("R_HandSocket"));
}

void AEnemy::SetCharacterSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

float AEnemy::GetCharacterSpeed() const
{
	return GetCharacterMovement()->GetMaxSpeed();
}

int AEnemy::MeleeAttack_Implementation()
{
	GetCharacterMovement()->StopActiveMovement();
	PlayAnimMontage(SwingMontage);
	
	return 0;
}

void AEnemy::Die() {

	// Disable movement and collision to prevent unintended behavior
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//play death anim
	GetMesh()->PlayAnimation(DeathAnim, false);
}

void AEnemy::HitAnimation() {
	
	//get Anim Insatnce
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	//play anim
	AnimInstance->Montage_Play(HitReactMontage);
}

void AEnemy::UpdateHealthBar(float CurrentHealth, float MaxHealth) {
	if (UUserWidget* Widget = HealthBarWidget->GetUserWidgetObject()) {
		if (UHealthBar* HealthBar = Cast<UHealthBar>(Widget)) {
			HealthBar->SetHealthProgressPrecent(CurrentHealth / MaxHealth);
		}
	}
}
// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	HealthComp->OnDeath.AddDynamic(this, &AEnemy::Die);
	HealthComp->OnDamaged.AddDynamic(this, &AEnemy::HitAnimation);
	HealthComp->OnHealthChanged.AddDynamic(this, &AEnemy::UpdateHealthBar);
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