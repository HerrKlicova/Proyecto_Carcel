#include "MainHUDWidget.h"
#include "Math/UnrealMathUtility.h"



void UMainHUDWidget::UpdateStaminaBar(float Current, float Max)
{
	{
		if (StaminaBar)
		{
			float percent = Current / Max;
		
			percent = FMath::Clamp(percent, 0.0f, 1.0f);
		
			StaminaBar->SetPercent(percent);
		}
	}
}

