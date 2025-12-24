// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatInterface.h"
#include "GameFramework/Character.h"
#include "HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimationAsset.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Sword.h"
#include "BehaviorTree/BehaviorTree.h"
#include "PatrolPath.h"

#include "Enemy.generated.h"

class UWidgetComponent;

UCLASS()
class MYPROJECT_API AEnemy : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void SetCharacterSpeed(float Speed);

	UPROPERTY(EditAnywhere, Category = "Movement")
	float IdleSpeed = 200.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float ChaseSpeed = 450.f;
	
	APatrolPath* GetPatrolPath() const { return PatrolPath; }
	
	UBehaviorTree* GetBehaviourTree() const { return EnemyTree; }
	
	UAnimMontage* GetAttackMontage() const {return SwingMontage;}
	
	float GetCharacterSpeed() const;
	
	int MeleeAttack_Implementation() override;

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
	
	bool AlreadyDied = false;

	UFUNCTION()
	void Die();

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
