// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "Pickup.generated.h"

class UItemBase;

UCLASS()
class MYPROJECT_API APickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	APickup();

	void InitializePickUp(const TSubclassOf<UItemBase> BaseClass, const int32 Quantity);

	void InitializeDrop(UItemBase* ItemtoDrop, const int32 InQuantity);

	FORCEINLINE UItemBase* GetItemData() const { return ItemReference; };

	virtual void BeginFocus() override;
	virtual void EndFocus() override;



protected:
	virtual void BeginPlay() override;

	virtual void Interact(AMyFirstCharacter* PlayerCharacter) override;

	void TakePickup(const AMyFirstCharacter* Taker);

	void UpdateInteractableData();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
#endif // WITH_EDITOR


	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	UDataTable* ItemDataTable;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	FName DesiredID;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Reference")
	UItemBase* ItemReference;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	int32 ItemQuantity;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	FInteractableData InstanceInteractableData;
};
