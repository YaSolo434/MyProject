// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"
#include "Components/ProgressBar.h"

void UHealthBar::SetHealthProgressPrecent(float Precent) {

	if (HealthProgressBar) {
		HealthProgressBar->SetPercent(FMath::Clamp(Precent, 0.f, 1.f));
		if (HealthProgressBar->GetPercent() <= 0.0f) {
			HealthProgressBar->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
