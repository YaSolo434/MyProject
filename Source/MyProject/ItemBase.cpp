// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "Kismet/GameplayStatics.h"
#include "InventoryComponent.h"


UItemBase::UItemBase() : bIsCopy(false), bIsPickup(false)
{
}

UItemBase* UItemBase::CreateItemCopy() const {

	UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass());

	ItemCopy->ID = this->ID;
	ItemCopy->Quantity = this->Quantity;
	ItemCopy->ItemQuality = this->ItemQuality;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->DescriptiveText = this->DescriptiveText;
	ItemCopy->NumericData = this->NumericData;
	ItemCopy->Statistics = this->Statistics;
	ItemCopy->AssetData = this->AssetData;
	ItemCopy->bIsCopy = true;
	
	return ItemCopy;
}

void UItemBase::ResetFlags() {
	bIsCopy = false;
	bIsPickup = false;
}

void UItemBase::SetQuantity(int32 NewQuantity) {
	
	if (NewQuantity != Quantity) {
		Quantity = FMath::Clamp(NewQuantity, 0, NumericData.bIsStackable ? NumericData.MaxStackSize : 1);
	}

	if (OwningInventory)
	{
		if (Quantity <= 0)
		{
			OwningInventory->RemoveSingleInstanceOfItem(this);
		}
	}
}

void UItemBase::Use(AMyFirstCharacter* Character) {
}

