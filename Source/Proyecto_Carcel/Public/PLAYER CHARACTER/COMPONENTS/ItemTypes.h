#pragma once

#include "CoreMinimal.h"
#include "ItemTypes.generated.h"

//	ENUM for the different types of items that will be on the game
UENUM(BlueprintType)
enum class EItemType : uint8
{
	None	UMETA(DisplayName = "None"),
	Weapon	UMETA(DisplayName = "Weapon"),
	Tool	UMETA(DisplayName = "Tool"),
	Key		UMETA(DisplayName = "Key"),
	Consumable	UMETA(DisplayName = "Consumable"),
};

//	STRUCT for the individual item on the inventary

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()
	//	Identifier for the Item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemID;
	//	Visible name for the object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemName;
	//	Type of item, defined by the Enum
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemType ItemType;
	//	Icon that will be shown on the HUD
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* ItemIcon;
	//	Value by defect for consumable objects like Health, Damage...
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float EffectValue = 0.0f;
};