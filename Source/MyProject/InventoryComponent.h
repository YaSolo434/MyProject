// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UItemBase;

UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
	IAR_NoItemAdded             UMETA(DisplayName = "No item added"),
	IAR_PartialAmountItemAdded	UMETA(DisplayName = "Partial amount of item added"),
	IAR_AllItemAdded			UMETA(DisplayName = "All of item added")
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_USTRUCT_BODY()

	FItemAddResult() : 
	ActualAmountAdded(0),
	OperationResult(EItemAddResult::IAR_NoItemAdded),
	ResultMessage(FText::GetEmpty())
	{};

	//actual ammount that added to inventory
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	int32 ActualAmountAdded;

	//enum representing the end state of an item add operation
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	EItemAddResult OperationResult;

	//a informational text that can be used for describing the result of process
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	FText ResultMessage;

	static FItemAddResult AddedNone(const FText& ErrorText) {

		FItemAddResult	AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = 0;
		AddedNoneResult.OperationResult = EItemAddResult::IAR_NoItemAdded;
		AddedNoneResult.ResultMessage = ErrorText;

		return AddedNoneResult;
	};

	static FItemAddResult AddedPartial (const int32 PartialAmountAdded, const FText& ErrorText) {

		FItemAddResult	AddedPartialResult;
		AddedPartialResult.ActualAmountAdded = PartialAmountAdded;
		AddedPartialResult.OperationResult = EItemAddResult::IAR_PartialAmountItemAdded;
		AddedPartialResult.ResultMessage = ErrorText;

		return AddedPartialResult;
	};

	static FItemAddResult AddedAll(const int32 AmountAdded, const FText& Message) {

		FItemAddResult	AddedAllResult;
		AddedAllResult.ActualAmountAdded = AmountAdded;
		AddedAllResult.OperationResult = EItemAddResult::IAR_AllItemAdded;
		AddedAllResult.ResultMessage = Message;

		return AddedAllResult;
	};

	
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	UItemBase* FindMatchingItem(UItemBase* ItemIn) const;
	UItemBase* FindNextItemById(UItemBase* ItemIn) const;
	UItemBase* FindNextPartialStack(UItemBase* ItemIn) const;

	FItemAddResult HandleAddItem(UItemBase* InputItem);

	void RemoveSingleInstanceOfItem(UItemBase* ItemIn);
	int32 RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove);

	void SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit);

	FORCEINLINE float GetInventoryTotalWeight() const {};
	FORCEINLINE float GetWeightCapacity() const {};
	FORCEINLINE int32 GetSlotsCapacity() const {};
	FORCEINLINE TArray<UItemBase*> GetInventoryContents() const {};

	FORCEINLINE	void SetSlotsCapacity(const int32 NewSlotsCapacity) {};
	FORCEINLINE void SetWeightCapacity(const float NewWeightCapacity) {};


protected:

	float InventoryTotalWeight;
	int32 InventorySlotsCapacity;
	float InventoryWeightCapacity;

	TArray<TObjectPtr<UItemBase>> InventoryContents;


	virtual void BeginPlay() override;

	FItemAddResult HandleNonStackableItems(UItemBase*, int32 RequestedAddAmount);
	int32 HandleStackableItems(UItemBase*, int32 RequestedAddAmount);

	int32 CalculateWeightAddAmount(UItemBase*, int32 RequestedAddAmount);
	int32 CalculateNumberForFullStack(UItemBase* ExistingItem, int32 InitialRequestedAddAmount);

	void AddNewItem(UItemBase* Item, int32 AmountToAdd);
};
