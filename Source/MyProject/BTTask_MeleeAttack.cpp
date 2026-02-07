// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MeleeAttack.h"

#include "AIController.h"
#include "Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
	NodeName = "Melee Attack";
	
	bNotifyTick = false;
	bCreateNodeInstance = false;
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* Memory = (FMeleeTaskMemory*)NodeMemory;
	Memory->bIsFinished = false;
	Memory->OwnerComp = &OwnerComp;
	
	AAIController* Controller = OwnerComp.GetAIOwner();
	AEnemy* Enemy = Controller ? Cast<AEnemy>(Controller->GetPawn()) : nullptr;
	if (!Enemy || Enemy->IsDead())
	{
		return EBTNodeResult::Failed;
	}
	
	UAnimInstance* AnimInstance = Enemy->GetMesh() ? Enemy->GetMesh()->GetAnimInstance() : nullptr;
	if (!AnimInstance)
	{
		return EBTNodeResult::Failed;
	}
	
	if (!Enemy->GetClass()->ImplementsInterface(UCombatInterface::StaticClass()))
	{
		return EBTNodeResult::Failed;
	}
	
	Memory->Enemy = Enemy;
	Memory->AnimInstance = AnimInstance;
	
	ICombatInterface* Combat = Cast<ICombatInterface>(Enemy);
	Combat->MeleeAttack();
	
	Enemy->GetCharacterMovement()->SetMovementMode(MOVE_None);
	
	UAnimMontage* AttackMontage = Enemy->GetAttackMontage();
	if (!AttackMontage)
	{
		return EBTNodeResult::Failed;
	}
	
	FOnMontageEnded Ended;
	Ended.BindUObject(this, &UBTTask_MeleeAttack::OnMontageEnded, NodeMemory);
	AnimInstance->Montage_SetEndDelegate(Ended, AttackMontage);
	
	return EBTNodeResult::InProgress;
}

void UBTTask_MeleeAttack::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted, uint8* NodeMemory)
{
	auto* Memory = (FMeleeTaskMemory*)NodeMemory;
	if (Memory->bIsFinished) {return;}
	
	Memory->bIsFinished = true;
	
	UBehaviorTreeComponent* BTC = Memory->OwnerComp.Get();
	AEnemy* Enemy = Memory->Enemy.Get();
	if (!BTC || !Enemy) {return;}
	
	Enemy->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	
	// Enemy->GetAttackBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	UBlackboardComponent* BB = BTC->GetBlackboardComponent();
	
	FinishLatentTask(*BTC, (!bInterrupted) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
}

EBTNodeResult::Type UBTTask_MeleeAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* Memory = (FMeleeTaskMemory*)NodeMemory;
	if (Memory->bIsFinished) {return EBTNodeResult::Aborted;}
	
	Memory->bIsFinished = true;
	
	if (AEnemy* Enemy = Memory->Enemy.Get())
	{
		Enemy->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		
		if (UAnimInstance* AnimInstance = Memory->AnimInstance.Get())
		{
			if (UAnimMontage* AttackMontage = Enemy->GetAttackMontage())
			{
				AnimInstance->Montage_Stop(0.1f, AttackMontage);
			}
		}
	}
	
	return EBTNodeResult::Aborted;
}


