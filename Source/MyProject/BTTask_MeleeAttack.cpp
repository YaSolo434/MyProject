// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MeleeAttack.h"

#include "AIController.h"
#include "Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
	NodeName = "Melee Attack";
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	
	AAIController* Controller = OwnerComp.GetAIOwner();
	AEnemy* Enemy = Cast<AEnemy>(Controller->GetPawn());

	if (!Enemy || Enemy->IsDead())
	{
		return EBTNodeResult::Failed;
	}

	UAnimInstance* AnimInstance = Enemy->GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return EBTNodeResult::Failed;
	}
	
	if (Enemy->GetClass()->ImplementsInterface(UCombatInterface::StaticClass()))
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Enemy))
		{
			CombatInterface->MeleeAttack();
			Enemy->GetCharacterMovement()->SetMovementMode(MOVE_None);
			
			FOnMontageBlendingOutStarted BlendingOutStarted;
			BlendingOutStarted.BindLambda([this, &OwnerComp, BB, Enemy](UAnimMontage* Montage, bool const bInterrupted = false)
			{
				if (!BB->GetValueAsBool(GetSelectedBlackboardKey()))
				{
					Enemy->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				}
				else
				{
					Enemy->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
					FinishLatentTask(OwnerComp, bInterrupted ? EBTNodeResult::Failed : EBTNodeResult::Succeeded);
				}
			});
			
			AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutStarted, Enemy->GetAttackMontage());
		}
	}
	
	return EBTNodeResult::InProgress;
}

