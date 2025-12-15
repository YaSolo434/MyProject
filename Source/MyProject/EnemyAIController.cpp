// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Enemy.h"

AEnemyAIController::AEnemyAIController(FObjectInitializer const& ObjectInitializer)
{
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AEnemy* const Enemy = Cast<AEnemy>(InPawn))
	{
		if (UBehaviorTree* const Tree = Enemy->GetBehaviourTree())
		{
			UBlackboardComponent* B;
			UseBlackboard(Tree->BlackboardAsset, B);
			Blackboard = B;
			RunBehaviorTree(Tree);
		}
	}
}
