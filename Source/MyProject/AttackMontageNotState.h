// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AttackMontageNotState.generated.h"

class AMyFirstCharacter;
UCLASS()
class MYPROJECT_API UAttackMontageNotState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;
};
