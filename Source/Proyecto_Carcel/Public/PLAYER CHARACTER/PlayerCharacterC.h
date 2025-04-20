#pragma once
///
///
///	libraries
#include "INTERFACES/DamageableInterface.h"
#include "INTERFACES/HealeableInterface.h"
#include "INTERFACES/IEquipableHandlerIterface.h"
#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "PlayerCharacterC.generated.h"
///
///
///	class forward declarations
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UMainHUDWidget;
///
///
///	structs forward declarations
struct FInputActionValue;
///
///
///	declare custom logging category
DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
///
///
///
///	declare enums | Stamina State
UENUM(Blueprintable)
enum class EStaminaState : uint8
{
	Normal	UMETA(DisplayName = "Normal"),
	Fatigued UMETA(DisplayName = "Fatigued"),
	Exhausted UMETA(DisplayName = "Exhausted"),
};
///	Movement State
UENUM(Blueprintable)
enum class EMovementState : uint8
{
	Standing UMETA(DisplayName = "Standing"),
	Walking UMETA(DisplayName = "Walking"),
	Running UMETA(DisplayName = "Running"),
};

UENUM(Blueprintable)
enum class EFieldOfViewState : uint8
{
	Normal	UMETA(DisplayName = "Normal"),
	Running	UMETA(DisplayName = "Running"),
	Fatigued UMETA(DisplayName = "Fatigued"),
};

UENUM(Blueprintable)
enum class EHoldingWeapon : uint8
{
	None	UMETA(DisplayName = "None"),
	Glock	UMETA(DisplayName = "Glock"),
	Knife	UMETA(DisplayName = "Knife")
};
///
//////////////////////////////////////////////////////
///
///


UCLASS()
class PROYECTO_CARCEL_API APlayerCharacterC : public ACharacter,
public IDamageableInterface,
public IHealeableInterface,
public IIEquipableHandlerIterface
{
	GENERATED_BODY()
	
	///////////////////////////////////////////////////
	///	COMPONENTS	COMPONENTS	COMPONENTS
	///	COMPONENTS	COMPONENTS	COMPONENTS
	///////////////////////////////////////////////////
	///
	///
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMesh = GetMesh();
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Inventory Component **/
	UPROPERTY(VisibleAnywhere)
	class UInventoryComponent* InventoryComponent;
	///
	///
	///////////////////////////////////////////////////
	///	INPUTS	INPUTS	INPUTS	INPUTS
	///	INPUTS	INPUTS	INPUTS	INPUTS
	///////////////////////////////////////////////////
	///
	///
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	
	/** Run Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RunAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	/*	Use Item Input Action*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* UseAction;
	
	/* Aim Weapon Input Action*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimAction;

	//	Input for selecting item from inventory
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SelectSlot0Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SelectSlot1Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SelectSlot2Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SelectSlot3Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SelectSlot4Action;
	///
	///
	///////////////////////////////////////////////////
	///	WIDGET	WIDGET	WIDGET
	///	WIDGET	WIDGET	WIDGET
	///////////////////////////////////////////////////
	///
	///	
public:
	//	Declares a variable UUserWidget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MainHUDWidgetClass;
	//	Declares a variable for instancing the HUD
	UPROPERTY()
	UMainHUDWidget* MainHUDWidgetInstance;
	///
	///
	///
	///
	/// Sets default values for this character's properties (constructor) */
	APlayerCharacterC();
	///
	///
	///
	/// Begin Play function */
	virtual void BeginPlay() override;
	/// Event Tick function */
	virtual void Tick(float DeltaTime) override;
	///
	///
	///
	//////////////////////////////////////////////////////
	///	STAMINA-SYSTEM-VAR	STAMINA-SYSTEM-VAR	STAMINA-SYSTEM-VAR
	///	STAMINA-SYSTEM-VAR	STAMINA-SYSTEM-VAR	STAMINA-SYSTEM-VAR
	/////////////////////////////////////////////////////
	///
	///
	/// Timer Handle Variable */
	FTimerHandle StaminaTimerHandle;
	///
	///
	///	Character's stamina, movement and Camera FOV states */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina|ENUMS")
	EStaminaState StaminaState = EStaminaState::Normal;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|ENUMS")
	EMovementState MovementState = EMovementState::Standing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|FOV")
	EFieldOfViewState FieldOfViewState = EFieldOfViewState::Normal;
	//
	//////////////////////////////////////////////////////
	///	CAMERA	CAMERA	CAMERA
	///	CAMERA	CAMERA	CAMERA
	/////////////////////////////////////////////////////
	///
	/**	Function called on the Tick that manages all the camera interpolation logic **/
	void UpdateCameraLag();
	
	/**	Function called on the Tick that manages the FOV changes **/
	void UpdateCameraFOV();
	
	//	Camera FOV variables
	float CurrentFOV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|FOV")
	float NormalFOV = 90.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|FOV")
	float RunningFOV = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|FOV")
	float FatiguedFOV = 85.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|FOV")
	float InterpFOVSpeed = 5.0;
	///
	/**	Create a FRotator for Camera Lag **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|FOV")
	FRotator TargetControlRotation;

	/**	Interpolation Speed for the lag camera effect	**/
	float InterpControlRotation = 15.0f;

	/**	Camera swaying on rotation variables **/
		/* Actual camera roll and camera roll target */
	float CurrentCameraRoll = 0.0f;
	float CameraRollTarget = 0.0f;

		/* Max swaying angle and interpolation speed */
	float MaxLeanAngle = 5.0f;
	float LeanInterpSpeed = 5.0f;
	
	///	Character's Max Movement Speeds */
	UPROPERTY()
	float characterSprintSpeed = 335.0f;
	UPROPERTY()
	float characterWalkSpeed = 130.0f;
	UPROPERTY()
	float characterSprintSpeedFatigued = 220.0f;
	///
	///
	///	Character's Stamina State Values */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float characterStamina = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float characterStaminaExhausted = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float characterStaminaTreshold = 30.0f;
	///
	///
	///	Character's Drain and Recovery Stamina Values */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina|Modifiers")
	float staminaToDrainRunning = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina|Modifiers")
	float staminaToRecoverWalkingFatigued = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina|Modifiers")
	float staminaToRecoverWalkingNotFatigued = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina|Modifiers")
	float staminaToRecoverNotWalkingNotFatigued = 15.0f;
	///
	///
	//////////////////////////////////////////////////////
	///	STAMINA-SYSTEM-FUNCTIONS	STAMINA-SYSTEM-FUNCTIONS
	///	STAMINA-SYSTEM-FUNCTIONS	STAMINA-SYSTEM-FUNCTIONS
	/////////////////////////////////////////////////////
	///
	///
	/// Function that works with a Timer and implements the Drain and Recovery Stamina system */
	UFUNCTION()
	void StaminaDrainAndRecovery();
	
	/// Function that drains a custom float of stamina to the player each Timer loop */
	UFUNCTION()
	void StaminaToDrainPerFloat(float& Stamina, float FloatToDrain);
	
	/// Function that add a custom float of stamina to the player each Timer loop */
	UFUNCTION()
	void StaminaToRecoverPerFloat(float& Stamina, float FloatToRecover);
	///
	///
	/////////////////////////////////////////////////////
	///	HEALTH-SYSTEM-VARS	HEALTH-SYSTEM-VARS
	///	HEALTH-SYSTEM-VARS	HEALTH-SYSTEM-VARS
	////////////////////////////////////////////////////////
	///
	///
	///	current health
	UPROPERTY(BlueprintReadWrite, Category = "Health")
	float CurrentHealth = 100.0f;
	///	max health
	UPROPERTY(BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0f;
	///
	///
	///////////////////////////////////////////////////////////
	///	HEALTH-SYSTEM-FUNCT	HEALTH-SYSTEM-FUNCT
	///	HEALTH-SYSTEM-FUNCT	HEALTH-SYSTEM-FUNCT
	///////////////////////////////////////////////////////////
	///
	
	///	Define the Interface Function from the DamageableInterface
	virtual void ApplyDamage_Implementation(float DamageAmount) override;
	///	Define the Interface Function from the HealeableInterface
	virtual void ApplyHealing_Implementation(float HealAmount) override;
	///

	///
	//////////////////////////////////////////////////////////////
	///	INTERACT-SYSTEM-VARS	INTERACT-SYSTEM-VARS
	///	INTERACT-SYSTEM-VARS	INTERACT-SYSTEM-VARS
	//////////////////////////////////////////////////////////////
	///
	///
	///	Declares the channel throught the Interact RayCast will be thrown
	UPROPERTY(EditAnywhere, Category = "Interact")
	TEnumAsByte<ECollisionChannel> InteractionChannelProperty = ECC_GameTraceChannel2;
	
	///	Declares the channel throught the Detection RayCast will be thrown
	UPROPERTY(EditAnywhere, Category = "Interact")
	TEnumAsByte<ECollisionChannel> DetecitonChannelProperty = ECC_GameTraceChannel3;
	
	//	Timer for detection LineTrace
	FTimerHandle DetectionTimerHandle;

	//	save the last detected actor for the HUD implementation (weak pointer cause is not essential for character)
	UPROPERTY()
	TWeakObjectPtr<AActor>LastDetectedActor = nullptr;
	///
	///
	/////////////////////////////////////////////////////////////////
	///	INTERACT-SYSTEM-FUNC		INTERACT-SYSTEM-FUNC
	///	INTERACT-SYSTEM-FUNC		INTERACT-SYSTEM-FUNC
	/////////////////////////////////////////////////////////////////
	///
	///
	
	//	Function that executes a Line Trace from camera forwards and interacts with actors who have interface
	UFUNCTION(BlueprintCallable, Category = "Interact")
	void InteractLineTrace();
	//	Function that executes a Line Trace from camera forwards and detects actors who have interface
	UFUNCTION(Blueprintable, Category = "Interact")
	void DetectionLineTrace();
	
	/////////////////////////////////////////////////////////////////
	///	INVENTORY-SYSTEM-VARS	INVENTORY-SYSTEM-VARS
	///	INVENTORY-SYSTEM-VARS	INVENTORY-SYSTEM-VARS
	/////////////////////////////////////////////////////////////////
	
	//	Init SelectedInventorySlot on unselected
	UPROPERTY()
	int32 SelectedInventorySlot = -1;

protected:
	/////////////////////////////////////////////////////
	///	INPUTS-FUNC	INPUTS-FUNC	INPUTS-FUNC	INPUTS-FUNC
	///	INPUTS-FUNC	INPUTS-FUNC	INPUTS-FUNC	INPUTS-FUNC
	////////////////////////////////////////////////////
	///
	///
	///	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called when movement is completed */
	UFUNCTION()
	void MovementCompleted();
	
	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for running input */
	void Run(const FInputActionValue& Value);

	/** Called for stop running input */
	void StopRunning(const FInputActionValue& Value);

	/** Called for interaction input */
	void Interact(const FInputActionValue& Value);
	
	/*	Called for selecting inventory items	*/
	void HandleSlotSelection(int32 SlotIndex);

	/*	Called for using the selected item	*/
	void UseInventoryItem();
	
	//	Reference to the equipped actor
	UPROPERTY()
	AActor* EquippedActor;
	
	//	Function that spawns a selected item
	void SpawnAndEquipItem(TSubclassOf<AActor> ItemClass);
	
	//	Function from the interface for equipping items
	virtual void EquipItemFromClass_Implementation(TSubclassOf<AActor> ItemClass, const FItemData& ItemData) override;
	
	//	Enum for different types of holding weapons
	UPROPERTY(BlueprintReadOnly, Category = "Weapons")
	EHoldingWeapon HoldingWeapon = EHoldingWeapon::None;
	
	//	Functions for selecting items on the inventory called
	void SelectSlot0();
	void SelectSlot1();
	void SelectSlot2();
	void SelectSlot3();
	void SelectSlot4();

	//Functions for aiming weapons
	void StartAiming(const FInputActionValue& Value);
	void StopAiming(const FInputActionValue& Value);
	
	//	Bool that changes when aiming or stops aiming
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	bool bIsAiming = false;
	
	/* Detect if controller has changed (inherited from AActor)	*/
	virtual void NotifyControllerChanged() override;

	/* Setup Inputs (Inherited from APawn)	*/
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
public:
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera;}
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const {return CameraBoom;}
	
};