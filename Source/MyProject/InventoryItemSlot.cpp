#include "InventoryItemSlot.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryTooltip.h"
#include "ItemBase.h"
#include "Components/TextBlock.h"

void UInventoryItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (TooltipClass)
	{
		UInventoryTooltip* Tooltip = CreateWidget<UInventoryTooltip>(this, TooltipClass);
		Tooltip->InventorySlotBeingHovered = this;
		SetToolTip(Tooltip);
	}
}

void UInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (ItemReference)
	{
        switch (ItemReference->ItemQuality)
        {
        case EItemQuality::Basic:
            ItemBorder->SetBrushColor(FLinearColor(0.8f, 0.8f, 0.8f)); // Gray
            break;

        case EItemQuality::Enchanted:
            ItemBorder->SetBrushColor(FLinearColor(0.2f, 0.6f, 1.f)); // Blue
            break;

        case EItemQuality::Superior:
            ItemBorder->SetBrushColor(FLinearColor(0.6f, 0.2f, 1.f)); // Purple
            break;

        case EItemQuality::MasterCrafted:
            ItemBorder->SetBrushColor(FLinearColor(1.f, 0.6f, 0.f)); // Orange
            break;

        case EItemQuality::GrandMaster:
            ItemBorder->SetBrushColor(FLinearColor(1.f, 0.8f, 0.f)); // Gold
            break;

        default:;
        }
	}

    ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);

    if (ItemReference->NumericData.bIsStackable)
    {
        ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));
    }
    else
    {
        ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
    }
}

FReply UInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
}

void UInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

}
