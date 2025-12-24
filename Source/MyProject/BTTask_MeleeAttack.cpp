// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MeleeAttack.h"

#include "AIController.h"
#include "Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
	NodeName = "Melee Attack";
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// if we are out of range then finish the task
	if (bool const OutOfRange = !OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey()))
	{
		//finish the task
		return EBTNodeResult::Succeeded;
	};
	
	// get ai and its controller
	AAIController const* const Controller = OwnerComp.GetAIOwner();
	AEnemy* const Enemy = Cast<AEnemy>(Controller->GetPawn());
	
	//check if it implements attackInterface
	if (ICombatInterface* const CombatInterface = Cast<ICombatInterface>(Enemy))
	{
		//check if it already is playing
		if (MontageFinished(Enemy))
		{
			CombatInterface->Execute_MeleeAttack(Enemy);
		}
	}
	
	return EBTNodeResult::Succeeded;
}

bool UBTTask_MeleeAttack::MontageFinished(AEnemy* const Enemy)
{
	return Enemy->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(Enemy->GetAttackMontage());
}
