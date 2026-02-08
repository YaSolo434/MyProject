// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFirstHUD.h"
#include "MainMenu.h"
#include "InteractionWidget.h"
#include "HealthBar.h"
#include "StaminaBar.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelSlot.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"

AMyFirstHUD::AMyFirstHUD() 
{
}

void AMyFirstHUD::BeginPlay() {

	Super::BeginPlay();

	if (MainMenuClass) 
	{
		MainMenu = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
		MainMenu->AddToViewport(5);
		MainMenu->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (InteractionWidgetClass) 
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	if (HealthBarClass)
	{
		HealthBarWidget = CreateWidget<UHealthBar>(GetWorld(), HealthBarClass);
		HealthBarWidget->AddToViewport(-1);
		HealthBarWidget->SetVisibility(ESlateVisibility::Visible);
		
		if (UProgressBar const* const ProgressBar = Cast<UProgressBar>(HealthBarWidget->GetWidgetFromName("HealthProgressBar")))
		{
			if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ProgressBar->Slot))
			{
				CanvasSlot->SetPosition(FVector2D(1460.f, 80.f));
				CanvasSlot->SetSize(FVector2D(350.f, 65.f));
				CanvasSlot->SetAnchors(FAnchors(0.f, 0.f));
			}
		}
	}
	
	if (StaminaBarWidgetClass) 
	{
		StaminaBarWidget = CreateWidget<UStaminaBar>(GetWorld(), StaminaBarWidgetClass);
		StaminaBarWidget->AddToViewport(-1);
		StaminaBarWidget->SetVisibility(ESlateVisibility::Visible);
	}
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

void AMyFirstHUD::ToggleMenu()
{
	if (IsMenuVisible)
	{
		HideMenu();

		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		DisplayMenu();

		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
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

void AMyFirstHUD::UpdateStaminaBar(float const CurSprintTime, float const MaxSprintTime) const
{
	if (StaminaBarWidget)
	{
		StaminaBarWidget->SetStaminaPrecent(CurSprintTime / MaxSprintTime);
	}
}