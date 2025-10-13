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
		SellValue->SetText(FText::Format(FText::FromString("Sell Value: {0} Crown"), FText::AsNumber(ItemBeingHovered->Statistics.Value)));
        break;

    case EItemType::Consumable:
		ItemType->SetText(FText::FromString("Consumable"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		SellValue->SetText(FText::Format(FText::FromString("Sell Value: {0} Crown"), FText::AsNumber(ItemBeingHovered->Statistics.Value)));
        break;

    case EItemType::Crafting:
		ItemType->SetText(FText::FromString("Crafting Material"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		UsageText->SetVisibility(ESlateVisibility::Collapsed);
		SellValue->SetText(FText::Format(FText::FromString("Sell Value: {0} Crown"), FText::AsNumber(ItemBeingHovered->Statistics.Value)));
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
		SellValue->SetText(FText::Format(FText::FromString("Sell Value: {0} Crown"), FText::AsNumber(ItemBeingHovered->Statistics.Value)));
        break;

    default: ;
    }

	ItemName->SetText(ItemBeingHovered->DescriptiveText.DisplayName);
	DamageValue->SetText(FText::AsNumber(ItemBeingHovered->Statistics.Damage));
	ArmorRating->SetText(FText::AsNumber(ItemBeingHovered->Statistics.Defence));
	UsageText->SetText(ItemBeingHovered->DescriptiveText.UsageInfo);
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
