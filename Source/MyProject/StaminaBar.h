// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StaminaBar.generated.h"

class UProgressBar;
UCLASS()
class MYPROJECT_API UStaminaBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetStaminaPrecent(float Precent);

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar;
	
};
