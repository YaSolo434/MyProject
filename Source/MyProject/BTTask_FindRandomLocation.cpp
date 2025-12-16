// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomLocation.h"
#include "EnemyAIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
    NodeName = TEXT("Find Random Location in NavMesh");
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    //get AI controller
    if (AEnemyAIController* const Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
    {
        //get its owner
        if (APawn* const Enemy = Controller->GetPawn())
        {
            //bind start location
            FVector const Orign = Enemy->GetActorLocation();

            //get the navigation system and generate a random location
            if (UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld())) 
            {
                //initalize end location
                FNavLocation Location;

                //bind it to blackboard value
                if (NavSystem->GetRandomPointInNavigableRadius(Orign, SearchRadius, Location))
                {
                    OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Location.Location);
                }
                
                return EBTNodeResult::Succeeded;
            }
        }
    }

    return EBTNodeResult::Failed;
}
