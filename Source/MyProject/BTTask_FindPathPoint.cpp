// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPathPoint.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy.h"

UBTTask_FindPathPoint::UBTTask_FindPathPoint(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Path Point");
}

EBTNodeResult::Type UBTTask_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		if (UBlackboardComponent* BC = OwnerComp.GetBlackboardComponent())
		{
			int32 const Index = BC->GetValueAsInt(GetSelectedBlackboardKey());

			if (AEnemy* Enemy = Cast<AEnemy>(Controller->GetPawn()))
			{
				FVector LocalPoint = Enemy->GetPatrolPath()->GetPatrolPoint(Index);

				FVector GlobalPoint = Enemy->GetPatrolPath()->GetActorTransform().TransformPosition(LocalPoint);

				BC->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);
				
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}