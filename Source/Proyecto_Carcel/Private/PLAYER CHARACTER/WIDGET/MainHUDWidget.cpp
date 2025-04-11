#include "PLAYER CHARACTER/WIDGET/MainHUDWidget.h"
#include "Math/UnrealMathUtility.h"



void UMainHUDWidget::UpdateStaminaBar(float Current, float Max)
{
	if (StaminaBar)
		{
			float percent = Current / Max;
		
			percent = FMath::Clamp(percent, 0.0f, 1.0f);
		
			StaminaBar->SetPercent(percent);
		}
	
}

void UMainHUDWidget::UpdateHealthbar(float Current, float Max)
{
	if (HealthBar)
	{
		float percent = Current / Max;

		percent = FMath::Clamp(percent, 0.0f, 1.0f);

		HealthBar->SetPercent(percent);
	}
}

void UMainHUDWidget::ShowInteractionMessage(FText Object)
{
	if (InteractMessage)
	{
		InteractMessage->SetText(Object);
		InteractMessage->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMainHUDWidget::HideInteractionMessage()
{
	if (InteractMessage)
	{
		InteractMessage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainHUDWidget::UpdateInventoryDisplay(const TArray<FItemData>& InventoryData)
{	//	create an array for itinerant the inventory slots
	const TArray<UImage*> SlotImages = {
	InventorySlot_0,
	InventorySlot_1,
	InventorySlot_2,
	InventorySlot_3,
	InventorySlot_4,
};
	//	create a for loop that itinerate all the inventory index and search if that index has an icon. If true, applies the selected texture on that slot.
	for (int32 i = 0; i < SlotImages.Num(); i++)
	{
		if (!SlotImages[i]) continue;

		if (InventoryData.IsValidIndex(i) && InventoryData[i].ItemIcon)
		{
			SlotImages[i]->SetBrushFromTexture(InventoryData[i].ItemIcon);
		}
		else
		{
			SlotImages[i]->SetBrushFromTexture(nullptr);
		}
	}
}
