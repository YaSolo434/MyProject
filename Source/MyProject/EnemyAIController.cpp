// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Enemy.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyFirstCharacter.h"

AEnemyAIController::AEnemyAIController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
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

void AEnemyAIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		SightConfig->SightRadius = 500.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
		SightConfig->PeripheralVisionAngleDegrees = 90.f;
		SightConfig->SetMaxAge(5.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;


		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}

}

void AEnemyAIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (AMyFirstCharacter* const PlayerCharacter = Cast<AMyFirstCharacter>(Actor))
	{
		const bool bSensed = Stimulus.WasSuccessfullySensed();

		Blackboard->SetValueAsBool("CanSeePlayer", bSensed);

		if (bSensed)
		{
			Blackboard->SetValueAsObject("TargetActor", PlayerCharacter);
		}
		else
		{
			Blackboard->ClearValue("TargetActor");
		}
	}
}