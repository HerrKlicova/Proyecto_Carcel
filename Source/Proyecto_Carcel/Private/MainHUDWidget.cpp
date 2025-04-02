#include "MainHUDWidget.h"
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
	}
}

void UMainHUDWidget::HideInteractionMessage()
{
}

