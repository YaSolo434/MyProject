// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_IsPlayerInMeleeRange.h"

#include "Enemy.h"
#include "EnemyAIController.h"
#include "MyFirstCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTService_IsPlayerInMeleeRange::UBTService_IsPlayerInMeleeRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = "Is Player In Range";
}

void UBTService_IsPlayerInMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get controller and ai
	AEnemyAIController* const Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	AEnemy* const Enemy = Cast<AEnemy>(Controller->GetPawn());
	
	//get player character
	AMyFirstCharacter* const Player = Cast<AMyFirstCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	//check distance
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(
		GetSelectedBlackboardKey(),
		Enemy->GetDistanceTo(Player) <= MeleeRange);
}
