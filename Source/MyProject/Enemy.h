#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatInterface.h" 
#include "DamageableInterface.h"

#include "Enemy.generated.h"

class UAnimationAsset;
class UAnimMontage;
class UBehaviorTree;
class UBoxComponent;
class UWidgetComponent;
class APatrolPath;
class UHealthComponent;


UCLASS()
class MYPROJECT_API AEnemy : public ACharacter, public ICombatInterface, public IDamageableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void SetCharacterSpeed(float Speed);

	UPROPERTY(EditAnywhere, Category = "Movement")
	float IdleSpeed = 200.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float ChaseSpeed = 450.f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Combat")
	bool bIsAttacking = false;
	
	UFUNCTION(BlueprintCallable)
	void EnableSwordHitbox();
	
	UFUNCTION(BlueprintCallable)
	void DisableSwordHitbox();
	
	UFUNCTION()
	void OnAttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	
	APatrolPath* GetPatrolPath() const { return PatrolPath; }
	
	UBehaviorTree* GetBehaviourTree() const { return EnemyTree; }
	
	UAnimMontage* GetAttackMontage() const {return SwingMontage;}
	
	float GetCharacterSpeed() const;
	
	virtual int32 MeleeAttack() override;
	
	bool IsDead() const { return bIsDead; }
	
	virtual void ApplyDamage(AActor* DamagedActor, float DamageAmount) override;
	
	virtual bool IsDamageable() const override;
	
	virtual bool IsActorDead(AActor* Actor) const override;
	
	virtual void ReceiveDamage(AActor* DamageCauser, float DamageAmount) override;
	
	virtual UHealthComponent* GetHealthComponent() const override;
	
	FORCEINLINE UBoxComponent* GetAttackBox() const {return AttackBox; }

protected:
	
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SwordMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimationAsset* DeathAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* HitReactMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* SwingMontage;
	
	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* AttackBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Damage = 1.f;

	UFUNCTION()
	void Die();
	
	bool bIsDead = false;
	
	UFUNCTION()
	void HitAnimation();

	UFUNCTION()
	void UpdateHealthBar(float CurrentHealth, float MaxHealth);

	UPROPERTY(EditAnywhere)
	UWidgetComponent* HealthBarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* EnemyTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	APatrolPath* PatrolPath;
};