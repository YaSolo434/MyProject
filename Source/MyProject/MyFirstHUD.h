// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyFirstHUD.generated.h"

class UMainMenu;
class UInteractionWidget;
class UHealthBar;

struct FInteractableData;

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyFirstHUD : public AHUD
{
	GENERATED_BODY()

public:
	//=================================================================================================
	// PROPERTIES & VARIABLES
	//=================================================================================================
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UMainMenu> MainMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UHealthBar> HealthBarClass;
	
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<class UStaminaBar> StaminaBarWidgetClass;

	bool IsMenuVisible;

	//=================================================================================================
	// FUNCTIONS
	//=================================================================================================
	AMyFirstHUD();

	void DisplayMenu();
	void HideMenu();
	void ToggleMenu();

	void ShowInteractionWidget() const;
	void HideInteractionWidget() const;
	void UpdateInteractionWidget(const FInteractableData* InteractableData) const;
	
	void UpdateStaminaBar(float const CurSprintTime, float const MaxSprintTime) const;

	FORCEINLINE UHealthBar* GetHealthBarWidget() const { return HealthBarWidget; };
	FORCEINLINE UStaminaBar* GetStaminaBarWidget() const { return StaminaBarWidget; };

protected:
	//=================================================================================================
	// PROPERTIES & VARIABLES
	//=================================================================================================
	UPROPERTY()
	UMainMenu* MainMenu;

	UPROPERTY()
	UInteractionWidget* InteractionWidget;
	
	UPROPERTY()
	UHealthBar* HealthBarWidget;
	
	UPROPERTY()
	UStaminaBar* StaminaBarWidget;

	//=================================================================================================
	// FUNCTIONS
	//=================================================================================================
	virtual void BeginPlay() override;

};
