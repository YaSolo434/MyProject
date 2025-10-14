// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaBar.h"
#include "Components/ProgressBar.h"

void UStaminaBar::SetStaminaPrecent(float Precent)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(FMath::Clamp(Precent, 0.f, 1.f));
	}
}
