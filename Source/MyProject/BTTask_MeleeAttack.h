// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MeleeAttack.generated.h"

class AEnemy;

USTRUCT()
struct FMeleeTaskMemory
{
	GENERATED_USTRUCT_BODY()
	
	TWeakObjectPtr<UAnimInstance> AnimInstance;
	TWeakObjectPtr<AEnemy> Enemy;
	TWeakObjectPtr<UBehaviorTreeComponent> OwnerComp;
	
	bool bIsFinished = false;
};


UCLASS()
class MYPROJECT_API UBTTask_MeleeAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_MeleeAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual uint16 GetInstanceMemorySize() const override {return sizeof(FMeleeTaskMemory); }

private:
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted, uint8* NodeMemory);
};
