// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryTooltip.h"
#include "InventoryItemSlot.h"
#include "ItemBase.h"
#include "Components/TextBlock.h" 

void UInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	// Check if slot has an item
	const UItemBase* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference();

    switch (ItemBeingHovered->ItemType)
    {
    case EItemType::Equipment:
		ItemType->SetText(FText::FromString("Equipment"));
        break;

    case EItemType::Consumable:
		ItemType->SetText(FText::FromString("Consumable"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
        break;

    case EItemType::Crafting:
		ItemType->SetText(FText::FromString("Crafting Material"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		UsageText->SetVisibility(ESlateVisibility::Collapsed);
        break;

    case EItemType::Quest:
		ItemType->SetText(FText::FromString("Quest Item"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		UsageText->SetVisibility(ESlateVisibility::Collapsed);
		SellValue->SetVisibility(ESlateVisibility::Collapsed);
        break;

    case EItemType::Junk:
		ItemType->SetText(FText::FromString("Junk"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		UsageText->SetVisibility(ESlateVisibility::Collapsed);
        break;

    default: ;
    }

	switch (ItemBeingHovered->ItemQuality)
	{
	case EItemQuality::Basic:
		ItemQuality->SetText(FText::FromString("Basic"));
		ItemQuality->SetColorAndOpacity(FSlateColor(FLinearColor(0.8f, 0.8f, 0.8f)));
		break;

	case EItemQuality::Enchanted:
		ItemQuality->SetText(FText::FromString("Enchanted"));
		ItemQuality->SetColorAndOpacity(FSlateColor(FLinearColor(0.2f, 0.6f, 1.f)));
		break;

	case EItemQuality::Superior:
		ItemQuality->SetText(FText::FromString("Superior"));
		ItemQuality->SetColorAndOpacity(FSlateColor(FLinearColor(0.6f, 0.2f, 1.f)));
		break;

	case EItemQuality::MasterCrafted:
		ItemQuality->SetText(FText::FromString("MasterCrafted"));
		ItemQuality->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 0.6f, 0.f)));
		break;

	case EItemQuality::GrandMaster:
		ItemQuality->SetText(FText::FromString("GrandMaster"));
		ItemQuality->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 0.8f, 0.f)));
		break;

	default: ;
	}

	ItemName->SetText(ItemBeingHovered->DescriptiveText.DisplayName);
	DamageValue->SetText(FText::AsNumber(ItemBeingHovered->Statistics.Damage));
	ArmorRating->SetText(FText::AsNumber(ItemBeingHovered->Statistics.Defence));
	UsageText->SetText(ItemBeingHovered->DescriptiveText.UsageInfo);
	UE_LOG(LogTemp, Warning, TEXT("Value = %d"), ItemBeingHovered->Statistics.Value);
	SellValue->SetText(FText::Format(FText::FromString("{0} Crown"), FText::AsNumber(ItemBeingHovered->Statistics.Value)));
	ItemDescription->SetText(ItemBeingHovered->DescriptiveText.Description);
	StackWeight->SetText(FText::AsNumber(ItemBeingHovered->GetItemStackWeight()));


	if (ItemBeingHovered->NumericData.bIsStackable)
	{
		MaxStackSize->SetText(FText::Format(FText::FromString("Max Stack Size: {0}"), FText::AsNumber(ItemBeingHovered->NumericData.MaxStackSize)));
	}
	else 
	{
		MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
	}
}
