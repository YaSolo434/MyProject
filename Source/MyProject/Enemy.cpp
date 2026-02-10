// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "HealthComponent.h"
#include "HealthBar.h"
#include "MyFirstCharacter.h"
#include "EnemyAIController.h"
#include "Blueprint/UserWidget.h"
#include "BrainComponent.h"


AEnemy::AEnemy() :
	 SwordMesh{CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"))},
	 HealthComp{CreateDefaultSubobject<UHealthComponent>(TEXT("Health"))},
	 AttackBox{CreateDefaultSubobject<UBoxComponent>(TEXT("Attack Box"))},
	 HealthBarWidget{CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"))}
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed = IdleSpeed;
	if (HealthComp)
	{
		HealthBarWidget->SetupAttachment(RootComponent);
		HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
		HealthBarWidget->SetDrawSize(FVector2D(100.f, 10.f));
		HealthBarWidget->SetInitialLayerZOrder(1);
		HealthBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
	}
	
	if (SwordMesh)
	{
		SwordMesh->SetupAttachment(GetMesh(), FName("R_HandSocket"));
	}
	
	if (AttackBox)
	{
		FAttachmentTransformRules const Rules{
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			false };
		
		AttackBox->AttachToComponent(GetMesh(), Rules, FName("R_HandSocket"));
		AttackBox->SetRelativeLocation(FVector(0.f, -58.f, 0.f));
		AttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AttackBox->SetCollisionProfileName(TEXT("EnemyAttack"));
	}
	

	bIsDead = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	HealthComp->OnDeath.AddDynamic(this, &AEnemy::Die);
	HealthComp->OnDamaged.AddDynamic(this, &AEnemy::HitAnimation);
	HealthComp->OnHealthChanged.AddDynamic(this, &AEnemy::UpdateHealthBar);
	
	AttackBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnAttackOverlap);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::SetCharacterSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void AEnemy::EnableSwordHitbox()
{
	if (bIsAttacking)
	{
		// bIsAttacking = false;
		AttackBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AEnemy::DisableSwordHitbox()
{
	AttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::OnAttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                             int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapped with character"));
	
	if (bIsAttacking && OtherActor && (OtherActor != this))
	{
		if (AMyFirstCharacter* Player = Cast<AMyFirstCharacter>(OtherActor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Applying Damage"));
			ApplyDamage(Player, Damage);
		}
	}
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
		
		// AttackBox->SetCollisionEnabled();
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
		//get Anim Instance
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