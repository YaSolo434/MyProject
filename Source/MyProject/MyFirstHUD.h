// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyFirstHUD.generated.h"

class UMainMenu;
class UInteractionWidget;

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

protected:
	//=================================================================================================
	// PROPERTIES & VARIABLES
	//=================================================================================================
	UPROPERTY()
	UMainMenu* MainMenu;

	UPROPERTY()
	UInteractionWidget* InteractionWidget;

	//=================================================================================================
	// FUNCTIONS
	//=================================================================================================
	virtual void BeginPlay() override;

};
