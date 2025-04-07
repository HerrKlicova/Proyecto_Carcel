//	includes
#include "PlayerCharacterC.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "Engine/World.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "InteractuableInterface.h"
#include "MainHUDWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Components/InputComponent.h"
////
///
//	Custom log category
DEFINE_LOG_CATEGORY(LogTemplateCharacter);
//
///
//// Sets default values (CONSTRUCTOR)
APlayerCharacterC::APlayerCharacterC()
{
	PrimaryActorTick.bCanEverTick = true;
	
	/**	Capsule Component Init **/
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	
	/**	Controller controls rotation **/
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	
	/**	Character Movement Rotation **/
	GetCharacterMovement()->bOrientRotationToMovement = false; //el jugador se mueve en la direccion al input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); //a esta rotación
	
	/**	Character Movement Basics **/ 
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	/**	Create camera Boom and adjust basics **/
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(SkeletalMesh, TEXT("headSocket"));
	CameraBoom->TargetArmLength = 2.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraRotationLagSpeed = 15.0f;
	
	/** Create camera component and attach to Camera Boom **/
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = true;

	
	/** Nota: Las referencias al skeletal mesh y el anim blueprint en el componente mesh (heredado de Character)
		son puestas en el blueprint asset derivado llamado ThirdPersonCharacter (para evitar referencias de contenido directas)	**/
}
////
///
//	begin play (HUD and Timers)
void APlayerCharacterC::BeginPlay()
{
	Super::BeginPlay();
	///
	///
	/**	Create HUD Widget to Viewport	**/
	if (MainHUDWidgetClass)
	{
		UUserWidget* CreatedWidget = CreateWidget<UUserWidget>(GetWorld(), MainHUDWidgetClass);

		UMainHUDWidget* CastedUserWidget = Cast<UMainHUDWidget>(CreatedWidget);

		if (CastedUserWidget)
		{
			MainHUDWidgetInstance = CastedUserWidget;
			CastedUserWidget->AddToViewport();
		}
	}
	///
	///
	/**	Init Current Health with Max Health	**/
	CurrentHealth = MaxHealth;
	
	/**	Update HUD health bar with CurrentHealth variable **/
	if (MainHUDWidgetInstance)
	{
		MainHUDWidgetInstance->UpdateHealthbar(CurrentHealth, MaxHealth);	
	}
	
	/**	Update HUD stamina bar with CharacterStamina variable **/
	if (MainHUDWidgetInstance)
	{
		MainHUDWidgetInstance->UpdateStaminaBar(characterStamina, 100.0f);
	}
	
	/**	Check if Player Character exists	**/
	if (IsValid(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{	/**	Init stamina timer 1.0f **/
		GetWorldTimerManager().SetTimer(
		StaminaTimerHandle,
		this,
		&APlayerCharacterC::StaminaDrainAndRecovery,
		1.0f,
		true
		);
		/**	Init detection timer 0.1f **/
		GetWorldTimerManager().SetTimer(
    	DetectionTimerHandle,
    	this,
    	&APlayerCharacterC::DetectionLineTrace,
    	0.1f,
    	true
    	);
	}
}
/**	Tick. Camera FOV and Camera Lag	**/
void APlayerCharacterC::Tick(float DeltaTime)
{	//	Tick (Camera FOV)
	Super::Tick(DeltaTime);

	/**	Calls the camera FOV function **/
	UpdateCameraFOV();

	/**	Calls the camera lag function **/
	UpdateCameraLag();
}
//
///
////
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///	INPUT MAPPING AND BINDING	INPUT MAPPING AND BINDING
///	INPUT MAPPING AND BINDING	INPUT MAPPING AND BINDING
//////////////////////////////////////////////////////////
////
///
//	Setup Input Mapping Context (cast to APlayerController)
void APlayerCharacterC::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}
//
///
////	Bind Input Actions
void APlayerCharacterC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving and stop moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacterC::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APlayerCharacterC::MovementCompleted);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacterC::Look);

		//Running
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &APlayerCharacterC::Run);

		//Stop Running
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &APlayerCharacterC::StopRunning);

		//Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APlayerCharacterC::Interact);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}
//
///
////	player starts moving...
void APlayerCharacterC::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// add movement
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		//	enum state to Walking
		MovementState = EMovementState::Walking;
	}
}
////
///
//		player stops moving...
void APlayerCharacterC::MovementCompleted()
{
	MovementState = EMovementState::Standing;
}
//
///
////	player move the mouse
void APlayerCharacterC::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller)
	{	//	Accumulates the rotation in TargetControlRotation
		TargetControlRotation.Yaw += LookAxisVector.X;
		TargetControlRotation.Pitch = FMath::Clamp(TargetControlRotation.Pitch + (LookAxisVector.Y * 1), -85.f, 85.f);

		/*	Detect where is rotating and adjust sway target	*/
		CameraRollTarget = LookAxisVector.X * -MaxLeanAngle;
		
		// add yaw and pitch input to controller and invert Y Axis
		//AddControllerYawInput(LookAxisVector.X);
		//AddControllerPitchInput(LookAxisVector.Y*-1);
	}
}
////
///
//		player starts running
void APlayerCharacterC::Run(const FInputActionValue& Value)
{	///	if movement state enum is walking...
	if (MovementState == EMovementState::Walking && GetVelocity().SizeSquared() > 0.0f)
	{	///	can start running
		MovementState = EMovementState::Running;
		
		// input is a Digital(Bool)
		bool bIsCharacterRunning = Value.Get<bool>();
        	
        	if (Controller != nullptr)
        	{	//	if exhausted...
        		if (StaminaState == EStaminaState::Exhausted)
        		{	//	stops moving
        			MovementState = EMovementState::Standing;
        			//FieldOfViewState = EFieldOfViewState::Normal;
        			GetCharacterMovement()->MaxWalkSpeed = characterWalkSpeed;
        		}	//	if fatigued...
        		else if (StaminaState == EStaminaState::Fatigued)
        		{	//	reduces speed
        			FieldOfViewState = EFieldOfViewState::Fatigued;
        			GetCharacterMovement()->MaxWalkSpeed = characterSprintSpeedFatigued;
        		}	//	if normal
        		else
        		{	//	runs
        			FieldOfViewState = EFieldOfViewState::Running;
        			GetCharacterMovement()->MaxWalkSpeed = characterSprintSpeed;
        		}
        	}
		
	}
}
//
///
////	player stops running
void APlayerCharacterC::StopRunning(const FInputActionValue& Value)
{
	bool bIsCharacterRunning = Value.Get<bool>();
	
	if (Controller != nullptr)
	{	//	if is NOT running 
		if (MovementState != EMovementState::Running)
        	{	//	starts walking
				MovementState = EMovementState::Walking;

				if (StaminaState == EStaminaState::Normal)
				{
					FieldOfViewState = EFieldOfViewState::Normal;
				}
				else
				{
					FieldOfViewState = EFieldOfViewState::Fatigued;
				}
			
        		GetCharacterMovement()->MaxWalkSpeed = characterWalkSpeed;
        	}
	}
}
////
///
//
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///	STAMINA-SYSTEM	STAMINA-SYSTEM	STAMINA-SYSTEM
///	STAMINA-SYSTEM	STAMINA-SYSTEM	STAMINA-SYSTEM
////////////////////////////////////////////////////
//
///
////	Drain stamina when running and recover stamina in different cuantities depending on movement state
void APlayerCharacterC::StaminaDrainAndRecovery()
{
	//	If character is running, drain stamina 
	if (MovementState == EMovementState::Running)
	{	// Function that decreases stamina depending on the state
		StaminaToDrainPerFloat(characterStamina, staminaToDrainRunning);
		
		//	if character stamina less or equal than 0
		if (characterStamina <= characterStaminaExhausted)
		{	//	is exhausted
			StaminaState = EStaminaState::Exhausted;
		}	//	if character stamina less than 30
		else if (characterStamina < characterStaminaTreshold)
		{	//	is fatigued
			StaminaState = EStaminaState::Fatigued;
			FieldOfViewState = EFieldOfViewState::Fatigued;
		}	//	if character stamina greater than 30
		else
		{	//	is normal
			StaminaState = EStaminaState::Normal;
		}
	}
	//	If character IS NOT running, recover stamina
	else
	{	//	If character is walking, he will recover stamina slower
		if (MovementState == EMovementState::Walking)
		{	// switch on Stamina State
			switch (StaminaState)
			{	//	on exhausted...
				case EStaminaState::Exhausted:
				//	on fatigued...
				case EStaminaState::Fatigued:
					//	recover 5.0f stamina
					StaminaToRecoverPerFloat(characterStamina, staminaToRecoverWalkingFatigued);
				break;
				// on normal...
				case EStaminaState::Normal:
					//	recover 10.0f stamina
					StaminaToRecoverPerFloat(characterStamina, staminaToRecoverWalkingNotFatigued);
				break;
			}
		}	// If character IS NOT moving, he will recover stamina a lot faster
		else
		{	//	recover 15.0f stamina
			StaminaToRecoverPerFloat(characterStamina, staminaToRecoverNotWalkingNotFatigued);
		}
		
	//	check on timer update and if player is NOT running, if stamina is greater than the treshold so the StaminaState is updated
	if (characterStamina >= characterStaminaTreshold)
	{
		StaminaState = EStaminaState::Normal;

		if (MovementState != EMovementState::Running)
		{
			FieldOfViewState = EFieldOfViewState::Normal;
		}
		else
		{
			FieldOfViewState = EFieldOfViewState::Running;
		}
	}		
	}

	////
	///
	//	implemented widget stamina bar functionality
	if (MainHUDWidgetInstance)
	{
		MainHUDWidgetInstance->UpdateStaminaBar(characterStamina, 100.0f);
	}
}
////	The stamina to drain when running
void APlayerCharacterC::StaminaToDrainPerFloat(float& Stamina, float FloatToDrain)
{
	Stamina = FMath::Clamp(Stamina - FloatToDrain, 0.0f, 100.0f);
}
////	The stamina to recover when walking depending the state
void APlayerCharacterC::StaminaToRecoverPerFloat(float& Stamina, float FloatToRecover)
{
	Stamina = FMath::Clamp(Stamina + FloatToRecover, 0.0f, 100.0f);
}
////
///
//
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///	DAMAGE-SYSTEM	DAMAGE-SYSTEM	DAMAGE-SYSTEM
///	DAMAGE-SYSTEM	DAMAGE-SYSTEM	DAMAGE-SYSTEM
////////////////////////////////////////////////////
//
/// 
////
void APlayerCharacterC::ApplyDamage_Implementation(float DamageAmount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);

	UE_LOG(LogTemplateCharacter, Warning, TEXT("El jugador ha recibido %.2f de daño. Salud actual: %.2f"), DamageAmount, CurrentHealth);

	if (CurrentHealth <= 0.0f)
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("JUGADOR MUERTO"));
	}

	if (MainHUDWidgetInstance)
	{
		MainHUDWidgetInstance->UpdateHealthbar(CurrentHealth, MaxHealth);	
	}
}
////
void APlayerCharacterC::ApplyHealing_Implementation(float HealAmount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.0f, MaxHealth);

	if (MainHUDWidgetInstance)
	{
		MainHUDWidgetInstance->UpdateHealthbar(CurrentHealth, MaxHealth);
	}
}
////
///
//
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///	INTERACT-SYSTEM	INTERACT-SYSTEM	INTERACT-SYSTEM
///	INTERACT-SYSTEM	INTERACT-SYSTEM	INTERACT-SYSTEM
////////////////////////////////////////////////////
//
///
/*	<-Called from the interact input-> */
void APlayerCharacterC::Interact(const FInputActionValue& Value)
{	//	get value of bIsInteracting Input
	bool bIsInteracting = Value.Get<bool>();

	//	If input is true, call the interact function
	if (bIsInteracting)
	{
		InteractLineTrace();
	}
}

/*	<-Called from interact function-> */
void APlayerCharacterC::InteractLineTrace()
{
	//	Struct that saves the info about the raycast impact (Actor, Location, Surface Normal...)
	FHitResult Hit;
	//	Raycast start point
	FVector Start = FollowCamera->GetComponentLocation();
	//	Raycast end point
	FVector End = FollowCamera->GetComponentLocation() + FollowCamera->GetForwardVector() * 500.0f;

	//	Creates a object with parameters for configuring the raycast (what will ignore, if has to detect complex objects...)
	FCollisionQueryParams QueryParams;
	//	Tells the raycast that has to ignore this actor
	QueryParams.AddIgnoredActor(this);
	
	//	Throws the raycast on the world from Start postion to End position using a collision channel (ECC_Pawn) and saves the result on Hit
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, InteractionChannelProperty, QueryParams);
	//	Paints a line of the raycast visible on the world. If hits something, the raycast will be Red, if not, will be green. 0.5 seconds with a thickness of 1
	DrawDebugLine(GetWorld(), Hit.TraceStart, Hit.TraceEnd, Hit.bBlockingHit ? FColor::Red : FColor::Green, false, 0.5f, 0, 1.0f);
	//	Shows on log the start point and end point of the raycast
	UE_LOG(LogTemplateCharacter, Warning, TEXT("Tracing line: %s to %s"), *Start.ToCompactString(), *End.ToCompactString());

	//	If hits and the actor hitted is Valid...
	if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
	{	//	...prints on screen the actor hitted
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Trace hit actor: %s"), *Hit.GetActor()->GetName()));
	}
	else
	{	//	...prints on screen no actor hitted
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("No hit actor"));

	}
	
	//	if hit actor Is Valid...
	if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
	{
		//	...create a variable with that actor...
		AActor* HitActor = Hit.GetActor();
		//	...and check if implements InteractuableInterface...
		if (HitActor->Implements<UInteractuableInterface>())
		{
			//	...execute the Interact function on the interactuable actor.
			IInteractuableInterface::Execute_Interact(HitActor);
		}
	}
}

/*	<-Called from the timer update-> */
void APlayerCharacterC::DetectionLineTrace()
{
	FHitResult Hit;

	FVector Start = FollowCamera->GetComponentLocation();
	FVector End = FollowCamera->GetComponentLocation() + FollowCamera->GetForwardVector() * 500.0f;
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	
	//	uses as channel the detection collision channel
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, DetecitonChannelProperty, QueryParams);

	//	check if Hit Actor blocked and if the actor blocked is valid
	if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
	{	//	save the Hit Actor on a AActor pointer
		AActor* CurrentActor = Hit.GetActor();
		
		//	if the detected actor is not equal to the last detected actor...
		if (CurrentActor != LastDetectedActor.Get() && CurrentActor->Implements<UInteractuableInterface>())
		{	//	...last detected actor will become the current actor
			LastDetectedActor = CurrentActor;
			
			/**	Show on HUD interaction message	using the interaction interface function **/
			FText Message = IInteractuableInterface::Execute_GetInteractionText(CurrentActor);
			MainHUDWidgetInstance->ShowInteractionMessage(Message);
		}
	}
	//	if the detected actor is the same as the last detected actor...
	else
	{	//	...clean the last detected actor pointer
		LastDetectedActor = nullptr;
		MainHUDWidgetInstance->HideInteractionMessage();
	}
	//DrawDebugLine(GetWorld(), Hit.TraceStart, Hit.TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Yellow, false, 0.1f, 0, 1.0f);
}

/*	This function Interpolates the camera Control Rotation to create a camera lag <-Called on Tick-> */
void APlayerCharacterC::UpdateCameraLag()
{
	//	Interpolates the accumulated rotation to the desired direction
	FRotator CurrentRotation = Controller->GetControlRotation();
	FRotator InterpolatedRotation = FMath::RInterpTo(CurrentRotation, TargetControlRotation, GetWorld()->GetDeltaSeconds(), InterpControlRotation);
	Controller->SetControlRotation(InterpolatedRotation);

	//	Interp from Current Roll to Target Roll
	CurrentCameraRoll = FMath::FInterpTo(CurrentCameraRoll, CameraRollTarget, GetWorld()->GetDeltaSeconds(), LeanInterpSpeed);
	
	//	Sets the relative rotation to the camera
	FRotator CameraRotation = CameraBoom->GetRelativeRotation();
	CameraRotation.Roll = CurrentCameraRoll;
	CameraBoom->SetRelativeRotation(CameraRotation);
}

/*	This function changes the camera FOV depending on the character movement and stamina state <-Called on Tick-> */
void APlayerCharacterC::UpdateCameraFOV()
{
	//	Set current FOV
	CurrentFOV = FollowCamera->FieldOfView;
	//	Target
	float TargetFOV;
	
	switch (FieldOfViewState)
	{	//	90.0f
	case EFieldOfViewState::Normal:
		TargetFOV = NormalFOV;
		break;	//	100.0f
	case EFieldOfViewState::Running:
		TargetFOV = RunningFOV;
		break;	//	85.0f
	case EFieldOfViewState::Fatigued:
		TargetFOV = FatiguedFOV;
		break;
	}	//	Set the FOV depending the FOV state (Interpolates from the current FOV to the target FOV. Interp Speed can be modified)
	FollowCamera->FieldOfView = FMath::FInterpTo(CurrentFOV, TargetFOV, GetWorld()->GetDeltaSeconds(), InterpFOVSpeed);
}

