#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "PLAYER CHARACTER/COMPONENTS/ItemTypes.h"
#include "MainHUDWidget.generated.h"


UCLASS()
class PROYECTO_CARCEL_API UMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

//**	
	//	declare a variable binded to the ProgressBar on the Widget BP
	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UProgressBar* StaminaBar;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UTextBlock* InteractMessage;
//**	
//**
	//	declare variables binded to the Inventory Slots images
	UPROPERTY(BlueprintReadOnly, Category = "Invetory", meta = (BindWidget))
	UImage* InventorySlot_0;

	UPROPERTY(BlueprintReadOnly, Category = "Invetory", meta = (BindWidget))
	UImage* InventorySlot_1;
	
	UPROPERTY(BlueprintReadOnly, Category = "Invetory", meta = (BindWidget))
	UImage* InventorySlot_2;
	
	UPROPERTY(BlueprintReadOnly, Category = "Invetory", meta = (BindWidget))
	UImage* InventorySlot_3;
	
	UPROPERTY(BlueprintReadOnly, Category = "Invetory", meta = (BindWidget))
	UImage* InventorySlot_4;
//**
	UFUNCTION(BlueprintCallable)
	void UpdateInventoryDisplay(const TArray<FItemData>& InventoryData);
//**	
	//	declare a function that will update the stamina bar
	UFUNCTION()
	void UpdateStaminaBar(float Current, float Max);

	UFUNCTION()
	void UpdateHealthbar(float Current, float Max);

	UFUNCTION()
	void ShowInteractionMessage(FText Message);

	UFUNCTION()
	void HideInteractionMessage();
//**
};
