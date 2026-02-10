// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackMontageNotState.h"
#include "MyFirstCharacter.h"

void UAttackMontageNotState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		if (AMyFirstCharacter* Player = Cast<AMyFirstCharacter>(MeshComp->GetOwner()))
		{
			Player->IsAttacking = false;
			Player->HasPlayedHitSound = false;
		}
	}
}
