// Fill out your copyright notice in the Description page of Project Settings.


#include "PLAYER CHARACTER/COMPONENTS/InventoryComponent.h"

#include "INTERFACES/HealeableInterface.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	//	Reserve space for 5 slots on the beginning
	InventorySlots.Reserve(MaxSlots);
	
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
//	Function that returns the success or fail of adding a new item to the inventory
bool UInventoryComponent::AddItem(const FItemData& NewItem)
{
	//	If we already have 5 items, we cant add more
	if (InventorySlots.Num() >= MaxSlots)
	{	//	Log and return the bool on false (failed)
		UE_LOG(LogTemp, Warning, TEXT("Inventario lleno. No se puede agregar %s"), *NewItem.ItemName);
		return false;
	}
	//	Add a new item to the inventory.
	InventorySlots.Add(NewItem);
	//	Log and return the bool on true (success)
	UE_LOG(LogTemp, Warning, TEXT("Nuevo objeto agregado al inventario %s"), *NewItem.ItemName);
	return true;
}

//	Function that applies the desired effect of the item
void UInventoryComponent::UseItem(int32 SlotIndex)
{
	//	Check if the index is valid
	if (!InventorySlots.IsValidIndex(SlotIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("SlotIndex is invalid %d"), SlotIndex);
		return;
	}
	
	//	Get the item slot and save it on the index
	const FItemData& Item = InventorySlots[SlotIndex];
	
	//	Apply logic depending on the item type
	switch (Item.ItemType)
	{
		//	if the item type is a consumable...
	case EItemType::Consumable:
		//	check if the component owner is the player...
		if (AActor* Owner = GetOwner())
		{	//	check if the player implements Healable Interface...
			if (Owner->GetClass()->ImplementsInterface(UHealeableInterface::StaticClass()))
			{	//	execute the interface function on the player.
				IHealeableInterface::Execute_ApplyHealing(Owner, Item.EffectValue);
			}
		}
		//	Remove the item if it is of single use.	
		RemoveItem(SlotIndex);
		break;
		//	if the item is a key...
	case EItemType::Key:
		//	remove the key from the inventory on use
		RemoveItem(SlotIndex);
		break;
		//	just log the item on the output log... logic will be implemented
	case EItemType::Weapon:
	case EItemType::Tool:
		UE_LOG(LogTemp, Warning, TEXT("Item type %d"), *Item.ItemName);
		break;
		//	if in any case the item is not recognized, just output log
	default:
		UE_LOG(LogTemp, Warning, TEXT("Not supported item %d"), static_cast<uint8>(Item.ItemType));
		break;
	}
}
void UInventoryComponent::RemoveItem(int32 SlotIndex)
{
	if (!InventorySlots.IsValidIndex(SlotIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("SlotIndex is invalid &d"), SlotIndex);
		return;
	}

	InventorySlots.RemoveAt(SlotIndex);
	UE_LOG(LogTemp, Warning, TEXT("Nuevo objeto removido %d"), SlotIndex);
}

