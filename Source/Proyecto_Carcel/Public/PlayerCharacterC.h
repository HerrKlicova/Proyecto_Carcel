#pragma once
///
///
///	libraries
#include "DamageableInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "PlayerCharacterC.generated.h"
///
///
///	classes
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UMainHUDWidget;
///
///
///	structs
struct FInputActionValue;
///
///
///	declare custom logging category
DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
///
///
///////////////////////////////////////////////////
///
UCLASS()
class PROYECTO_CARCEL_API APlayerCharacterC : public ACharacter, public IDamageableInterface
{
	GENERATED_BODY()
	
	///////////////////////////////////////////////////
	///	COMPONENTS	COMPONENTS	COMPONENTS
	///	COMPONENTS	COMPONENTS	COMPONENTS
	///////////////////////////////////////////////////
	///
	///
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
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
	///
	///
	///////////////////////////////////////////////////
	///	WIDGET	WIDGET	WIDGET
	///	WIDGET	WIDGET	WIDGET
	///////////////////////////////////////////////////
	///
	///	Add variables for creating widget
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MainHUDWidgetClass;
	
	UPROPERTY()
	UMainHUDWidget* MainHUDWidgetInstance;
	///
	///
public:
	///
	///
	/// Sets default values for this character's properties (constructor) */
	APlayerCharacterC();
	///
	///
	///
	/// Begin Play function */
	virtual void BeginPlay() override;
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
	FTimerHandle TimerHandleStamina;
	///
	///
	///	Character's Bools */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCharacterMoving = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCharacterFatigued = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCharacterExhausted = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCharacterRunning;
	///
	///
	///	Character's Max Walk Speeds */
	UPROPERTY()
	float characterSprintSpeed = 800.0f;
	UPROPERTY()
	float characterWalkSpeed = 300.0f;
	UPROPERTY()
	float characterSprintSpeedFatigued = 600.0f;
	///
	///
	///	Character's Stamina State Values */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float characterStamina = 100.0f;
	UPROPERTY()
	float characterStaminaExhausted = 0.0f;
	UPROPERTY()
	float characterStaminaTreshold = 30.0f;
	///
	///
	///	Character's Drain and Recovery Stamina Values */
	UPROPERTY()
	float staminaToDrainRunning = 10.0f;
	UPROPERTY()
	float staminaToRecoverWalkingFatigued = 5.0f;
	UPROPERTY()
	float staminaToRecoverWalkingNotFatigued = 10.0f;
	UPROPERTY()
	float staminaToRecoverNotWalkingNotFatigued = 15.0f;
	///
	///
	//////////////////////////////////////////////////////
	///	STAMINA-SYSTEM-FUNCTIONS	STAMINA-SYSTEM-FUNCTIONS
	///	STAMINA-SYSTEM-FUNCTIONS	STAMINA-SYSTEM-FUNCTIONS
	/////////////////////////////////////////////////////

	
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
	UPROPERTY(BlueprintReadOnly)
	float CurrentHealth = 100.0f;
	///	max health
	UPROPERTY(BlueprintReadOnly)
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
	///
	///
	///
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

	/* Detect if controller has changed (inherited from AActor)	*/
	virtual void NotifyControllerChanged() override;

	/* Setup Inputs (Inherited from APawn)	*/
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	
};
