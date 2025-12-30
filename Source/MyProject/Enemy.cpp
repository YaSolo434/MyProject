// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"

#include "BTTask_MeleeAttack.h"
#include "EnemyAIController.h"
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
	
	AttackBox = CreateDefaultSubobject<UBoxComponent>("Attack Box");
	AttackBox->SetupAttachment(GetMesh(), FName("R_HandSocket"));
	
	bIsDead = false;
}

void AEnemy::SetCharacterSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

float AEnemy::GetCharacterSpeed() const
{
	return GetCharacterMovement()->GetMaxSpeed();
}

int32 AEnemy::MeleeAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("MeleeAttack_Implementation CALLED"));
	
	if (SwingMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		
		AnimInstance->Montage_Play(SwingMontage);
	}
	return 0;	
}

void AEnemy::ApplyDamage(AActor* DamagedActor, float const DamageAmount)
{
	if (DamagedActor && DamageAmount > 0.f)
	{
		IDamageableInterface* Damageable = Cast<IDamageableInterface>(DamagedActor);
		
		if (Damageable && Damageable->IsDamageable())
		{
			Damageable->ReceiveDamage(this, DamageAmount);
		}
	}
}

bool AEnemy::IsDamageable() const
{
	return !bIsDead;
}

bool AEnemy::IsActorDead(AActor* Actor) const
{
	return bIsDead;
}

void AEnemy::ReceiveDamage(AActor* DamageCauser, float const DamageAmount)
{
	if (HealthComp && DamageCauser && DamageAmount > 0.f)
	{
		HealthComp->TakeDamage(DamageAmount);
	}
}

UHealthComponent* AEnemy::GetHealthComponent() const
{
	return HealthComp;
}

void AEnemy::Die() {

	if (bIsDead){return;}
	
	bIsDead = true;
	
	// Stop AI behavior and unpossess controller
	if (AEnemyAIController* EnemyController = Cast<AEnemyAIController>(GetController()))
	{
		if (EnemyController->BrainComponent)
		{
			EnemyController->BrainComponent->StopLogic(TEXT("Enemy Dead"));
		}
	}
	
	// Disable movement
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	
	// Disable visibility signal
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	
	if (UAnimInstance* AnimIns = GetMesh()->GetAnimInstance())
	{
		AnimIns->StopAllMontages(0.1f);
	}
	// Disable collision to prevent unintended behavior
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// Hide health bar
	HealthBarWidget->SetVisibility(false);
	
	//play death anim
	GetMesh()->PlayAnimation(DeathAnim, false);
	
	SetLifeSpan(5.f);
}

void AEnemy::HitAnimation() {
	if (!bIsDead)
	{
		//get Anim Insatnce
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		//play anim
		AnimInstance->Montage_Play(HitReactMontage);
	}
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
	
	// PlayAnimMontage(SwingMontage);
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