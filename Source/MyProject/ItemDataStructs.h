// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataStructs.generated.h"

UENUM()
enum class EItemQuality : uint8
{
	Basic			UMETA(DisplayName = "Basic"),
	Enchanted		UMETA(DisplayName = "Enchanted"),
	Superior		UMETA(DisplayName = "Superior"),
	MasterCrafted	UMETA(DisplayName = "MasterCrafted"),
	GrandMaster		UMETA(DisplayName = "GrandMaster")
};

UENUM()
enum class EItemType : uint8
{
	Equipment       UMETA(DisplayName = "Equipment"),
	Consumable      UMETA(DisplayName = "Consumable"),
	Crafting        UMETA(DisplayName = "Crafting Material"),
	Quest           UMETA(DisplayName = "Quest Item"),
	Junk            UMETA(DisplayName = "Junk")
	
};

USTRUCT()
struct FItemStatistics
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float Damage;

	UPROPERTY(EditAnywhere)
	float AttackSpeed;

	UPROPERTY(EditAnywhere)
	float CriticalChance;

	UPROPERTY(EditAnywhere)
	float Range;

	UPROPERTY(EditAnywhere)
	float Defence;

	UPROPERTY(EditAnywhere)
	int32 HealAmount;

	UPROPERTY(EditAnywhere)
	int32 Value;
};

USTRUCT()
struct FItemDescriptiveTextData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FText DisplayName;

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	FText UsageInfo;

	UPROPERTY(EditAnywhere)
	FText InteractionText;
};

USTRUCT()
struct FItemNumericData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float Weight;

	UPROPERTY(EditAnywhere)
	int32 MaxStackSize;

	UPROPERTY(EditAnywhere)
	bool bIsStackable;
};

USTRUCT()
struct FItemAssetData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;

};

USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

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
};
