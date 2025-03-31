#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "MainHUDWidget.generated.h"


UCLASS()
class PROYECTO_CARCEL_API UMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	
	//	declare a variable binded to the ProgressBar on the Widget BP
	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UProgressBar* StaminaBar;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UProgressBar* HealthBar;

	//	declare a function that will update the stamina bar
	UFUNCTION()
	void UpdateStaminaBar(float Current, float Max);

	UFUNCTION()
	void UpdateHealthbar(float Current, float Max);
};
