// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "ItemBase.h"
#include "MyFirstCharacter.h"
#include "InventoryComponent.h"
// Sets default values
APickup::APickup()
{

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	RootComponent = PickupMesh;
	PickupMesh->SetSimulatePhysics(true);

}

void APickup::InitializePickUp(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity) {

	if (ItemDataTable && !DesiredID.IsNone()) {
		
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredID, DesiredID.ToString());

		ItemReference = NewObject<UItemBase>(this, BaseClass);

		ItemReference->ID = ItemData->ID;
		ItemReference->ItemType = ItemData->ItemType;
		ItemReference->ItemQuality = ItemData->ItemQuality;
		ItemReference->NumericData = ItemData->NumericData;
		ItemReference->DescriptiveText = ItemData->DescriptiveText;
		ItemReference->AssetData = ItemData->AssetData;

		InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

		PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);
		
		UpdateInteractableData();
	}
}

void APickup::InitializeDrop(UItemBase* ItemtoDrop, const int32 InQuantity) {

	ItemReference = ItemtoDrop;
	InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
	ItemReference->NumericData.Weight = ItemtoDrop->GetItemSingleWeight();
	PickupMesh->SetStaticMesh(ItemtoDrop->AssetData.Mesh);

	UpdateInteractableData();		
}

void APickup::UpdateInteractableData() {
	InstanceInteractableData.InteractableType = EInteractableType::PickUp;
	InstanceInteractableData.Action = ItemReference->DescriptiveText.InteractionText;
	InstanceInteractableData.Name = ItemReference->DescriptiveText.DisplayName;
	InstanceInteractableData.Quantity = ItemReference->Quantity;
	InteractableData = InstanceInteractableData;
}


void APickup::BeginFocus() {
	if (PickupMesh) {

		PickupMesh->SetRenderCustomDepth(true);
	}	
}

void APickup::EndFocus() {
	if (PickupMesh) {

		PickupMesh->SetRenderCustomDepth(false);
	}
}

void APickup::Interact(AMyFirstCharacter* PlayerCharacter) {
	if (PlayerCharacter) {

		TakePickup(PlayerCharacter);
	}

}

void APickup::TakePickup(const AMyFirstCharacter* Taker) {

	if (!IsPendingKillPending()) {
		if (ItemReference) {
			if (UInventoryComponent* PlayerInventory = Taker->GetInventory()) {
				const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);

				switch (AddResult.OperationResult)
				{
				case EItemAddResult::IAR_NoItemAdded:
					break;
				case EItemAddResult::IAR_PartialAmountItemAdded:
					UpdateInteractableData();
					Taker->UpdateInteractionWidget();
					break;
				case EItemAddResult::IAR_AllItemAdded:
					Destroy();
					break;
				}

				UE_LOG(LogTemp, Warning, TEXT("%s"), *AddResult.ResultMessage.ToString());
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Player inventory is null"));
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Item reference is somehow null"));
		}
	}
}

void APickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(APickup, DesiredID)) {

		if (ItemDataTable) {
			
			if (const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredID, DesiredID.ToString())) {

				PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);
			}
		}
	}
}


// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

	InitializePickUp(UItemBase::StaticClass(), ItemQuantity);
	
}
