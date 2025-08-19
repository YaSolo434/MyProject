// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"

void UHealthBar::SetHealthPrecent(float Precent) {

	if (HealthBar) {
		HealthBar->SetPercent(FMath::Clamp(Precent, 0.f, 1.f));
	}
}
