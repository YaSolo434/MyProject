// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IncrementPathIndex.h"
#include "Enemy.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_IncrementPathIndex::UBTTask_IncrementPathIndex(FObjectInitializer const& ObjectInitializer) :
	UBTTask_BlackboardBase{ ObjectInitializer }
{
	NodeName = TEXT("Increment Path Index");
}

EBTNodeResult::Type UBTTask_IncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// try to get AI controlller
	if (AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetOwner()))
	{
		// try to get AI itself
		if (AEnemy* Enemy = Cast<AEnemy>(Controller->GetPawn()))
		{
			// try to get AI Blackboard
			if (UBlackboardComponent* BC = OwnerComp.GetBlackboardComponent())
			{
				// get Max Number of points from PatrolPath
				int const NumberOfPoints = Enemy->GetPatrolPath()->GetNum();
				int const MinIndex = 0;
				// decrement bec of index
				int const MaxIndex = NumberOfPoints - 1;
				//Get current index
				int32 Index = BC->GetValueAsInt(GetSelectedBlackboardKey());
				
				// Handle bidirectional patrol.Reverse direction at path endpoints
				if (bBiDirectional)
				{
					if (Index >= MaxIndex && Direction == EDirectionType::Forward)
					{
						Direction = EDirectionType::Reverse;
					}
					else if (Index == MinIndex && Direction == EDirectionType::Reverse)
					{
						Direction = EDirectionType::Forward;
					}
				}

				// set index value to the current one
				BC->SetValueAsInt(GetSelectedBlackboardKey(), 
					(Direction == EDirectionType::Forward ? ++Index : --Index) % NumberOfPoints);

				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}