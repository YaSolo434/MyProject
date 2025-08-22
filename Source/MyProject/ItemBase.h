// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataStructs.h"

#include "ItemBase.generated.h"

class AMyFirstCharacter;

UCLASS()
class MYPROJECT_API UItemBase : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, Category = "ItemData", meta = (UIMin = 1, UIMax = 100))
	int32 Quantity;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	EItemQuality ItemQuality;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	FItemStatistics Statistics;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	FItemDescriptiveTextData DescriptiveText;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	FItemNumericData NumericData;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	FItemAssetData AssetData;


	UItemBase();

	UFUNCTION(Category = "Item")
	UItemBase* CreateItemCopy() const;

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemStackWeight() const { return Quantity * NumericData.Weight; };

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const { return NumericData.Weight; };

	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsFullItemStack() const { return Quantity == NumericData.MaxStackSize; };

	UFUNCTION(Category = "Item")
	void SetQuantity(int32 NewQuantity);

	UFUNCTION(Category = "Item")
	virtual void Use(AMyFirstCharacter* Character);

protected:
	bool operator==(const FName& OtherID) const {

		return ID == OtherID;
	}
};
