// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFirstHUD.h"
#include "MainMenu.h"
#include "InteractionWidget.h"

AMyFirstHUD::AMyFirstHUD() {

	
}

void AMyFirstHUD::DisplayMenu() {

	if (MainMenu) {

		IsMenuVisible = true;
		MainMenu->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMyFirstHUD::HideMenu() {

	if (MainMenu) {

		IsMenuVisible = false;
		MainMenu->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AMyFirstHUD::ShowInteractionWidget() const {

	if (InteractionWidget) {
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMyFirstHUD::HideInteractionWidget() const {

	if (InteractionWidget) {
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AMyFirstHUD::UpdateInteractionWidget(const FInteractableData* InteractableData) const {

	if (InteractionWidget) {
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed) {

			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}

		InteractionWidget->UpdateWidget(InteractableData);
	}

}

void AMyFirstHUD::BeginPlay() {

	Super::BeginPlay();

	if (MainMenuClass) {
		MainMenu = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
		MainMenu->AddToViewport(5);
		MainMenu->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (InteractionWidgetClass) {
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}
