// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PLAYER CHARACTER/COMPONENTS/ItemTypes.h"
#include "InteractuableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractuableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROYECTO_CARCEL_API IInteractuableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**	Interact function	**/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	void Interact();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	FItemData GetItemData();
	
	/**	Show Item Name on HUD function	**/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	FText GetInteractionText();
};
