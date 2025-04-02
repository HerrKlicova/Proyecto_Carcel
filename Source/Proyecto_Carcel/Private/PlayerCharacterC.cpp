//	includes
#include "PlayerCharacterC.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "Engine/World.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InteractuableInterface.h"
#include "MainHUDWidget.h"
#include "Blueprint/UserWidget.h"
#include "Math/UnrealMathUtility.h"
#include "Components/InputComponent.h"
///
///
///	Custom log category
DEFINE_LOG_CATEGORY(LogTemplateCharacter);
///
///
///
/// Sets default values (CONSTRUCTOR)
APlayerCharacterC::APlayerCharacterC()
{
	//Inicializar el tamaño de la cápsula de personaje
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	
	//No rotar cuando el controlador rote
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Configurar movimiento del character
	GetCharacterMovement()->bOrientRotationToMovement = true; //el jugador se mueve en la direccion al input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); //a esta rotación

	//Ajustes básicos del character movement 
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	//Crear el brazo (Se acerca al jugador si detecta colisión)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; //La camara se quedará a esta distancia detrás del jugador
	CameraBoom->bUsePawnControlRotation = true; //Rotar el brazo en base al controlador

	//Crear camara que sigue al jugador
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); //Enganchar la camara al final del camera boom
	FollowCamera->bUsePawnControlRotation = false; //La camara no rota de forma relativa al brazo

	// Nota: Las referencias al skeletal mesh y el anim blueprint en el componente mesh (heredado de Character)
	// son puestas en el blueprint asset derivado llamado ThirdPersonCharacter (para evitar referencias de contenido directas)
	
}
///
///
///begin play (HUD and Timers)
void APlayerCharacterC::BeginPlay()
{
	Super::BeginPlay();
	///
	///
	//Create HUD Widget to Viewport
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
	///	Init Current Health with Max Health
	CurrentHealth = MaxHealth;
	if (MainHUDWidgetInstance)
	{
		MainHUDWidgetInstance->UpdateHealthbar(CurrentHealth, MaxHealth);	
	}
	///
	///
	///
	///
	if (MainHUDWidgetInstance)
	{
		MainHUDWidgetInstance->UpdateStaminaBar(characterStamina, 100.0f);
	}
	///
	///
	//Timer for stamina Drain and Recovery
	GetWorldTimerManager().SetTimer(
		TimerHandleStamina,
		this,
		&APlayerCharacterC::StaminaDrainAndRecovery,
		1.0f,
		true
		);
}

void APlayerCharacterC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

///
///
///
/////////////////////////////////////////////////////
///	INPUT MAPPING AND BINDING	INPUT MAPPING AND BINDING
///	INPUT MAPPING AND BINDING	INPUT MAPPING AND BINDING
////////////////////////////////////////////////////
///
///
///	Setup Input Mapping Context (cast to APlayerController)
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
///
///
///	Bind Input Actions
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
///
///
///	player starts moving...
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

		//if character is moving in any direction, it will face towards look direction
		GetCharacterMovement()->bOrientRotationToMovement = false;

		bIsCharacterMoving = true;
	}
}
///
///
///	player stops moving...
void APlayerCharacterC::MovementCompleted()
{
	//so will stop facing towards look direction
	GetCharacterMovement()->bOrientRotationToMovement = true;

	bIsCharacterMoving = false;
	bIsCharacterRunning = false;
}
///
///
///	player stops looking
void APlayerCharacterC::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller and invert Y Axis
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y*-1);
	}
	
	
}
///
///
///	player starts running
void APlayerCharacterC::Run(const FInputActionValue& Value)
{
	if (bIsCharacterMoving)
	{
		// input is a Digital(Bool)
		bIsCharacterRunning = Value.Get<bool>();
        	
        	if (Controller != nullptr)
        	{
        		if (bIsCharacterExhausted && bIsCharacterFatigued)
        		{
        			bIsCharacterRunning = false;
        			GetCharacterMovement()->MaxWalkSpeed = characterWalkSpeed;
        		}
        		else
        		{
        			GetCharacterMovement()->MaxWalkSpeed = bIsCharacterFatigued ? characterSprintSpeedFatigued : characterSprintSpeed;
        		}
        	}
	}
}
///
///
///	player stops running
void APlayerCharacterC::StopRunning(const FInputActionValue& Value)
{
	bIsCharacterRunning = Value.Get<bool>();
	
	if (Controller != nullptr)
	{
		if (!bIsCharacterRunning)
        	{
        		GetCharacterMovement()->MaxWalkSpeed = characterWalkSpeed;
        	}
	}
}
///
///
////////////////////////////////////////////////////////
///	STAMINA-SYSTEM	STAMINA-SYSTEM	STAMINA-SYSTEM
///	STAMINA-SYSTEM	STAMINA-SYSTEM	STAMINA-SYSTEM
////////////////////////////////////////////////////
///
///
///	Drain stamina when running and recover stamina in different cuantities depending on movement state
void APlayerCharacterC::StaminaDrainAndRecovery()
{
	//	If character is running, drain stamina 
	if (bIsCharacterRunning)
	{	// Function that decreases stamina 
		StaminaToDrainPerFloat(characterStamina, staminaToDrainRunning);
		//	Character fatigued and exhausted		
		if (characterStamina < characterStaminaTreshold)
		{
			bIsCharacterFatigued = true;
			
			if (characterStamina <= characterStaminaExhausted)
			{
				bIsCharacterExhausted = true;
			}
		}
	}	//	If character IS NOT running, recover stamina
	else
	{	//	If character is moving, he will recover stamina slower
		if (bIsCharacterMoving)
		{
			StaminaToRecoverPerFloat(characterStamina,
				(characterStamina <= characterStaminaTreshold) ?
				staminaToRecoverWalkingFatigued : staminaToRecoverWalkingNotFatigued);
			
			if (characterStamina > characterStaminaExhausted && characterStamina >= characterStaminaTreshold)
			{
				bIsCharacterExhausted = false;
			}
			
			if (characterStamina >= characterStaminaTreshold)
			{
				bIsCharacterFatigued = false;
			}
		}	// If character IS NOT moving, he will recover stamina a lot faster
		else
		{
			StaminaToRecoverPerFloat(characterStamina, staminaToRecoverNotWalkingNotFatigued);
		}
	}
	///
	///
	//	implemented widget stamina bar functionality
	if (MainHUDWidgetInstance)
	{
		MainHUDWidgetInstance->UpdateStaminaBar(characterStamina, 100.0f);
	}
}

void APlayerCharacterC::StaminaToDrainPerFloat(float& Stamina, float FloatToDrain)
{
	Stamina = FMath::Clamp(Stamina - FloatToDrain, 0.0f, 100.0f);
}

void APlayerCharacterC::StaminaToRecoverPerFloat(float& Stamina, float FloatToRecover)
{
	Stamina = FMath::Clamp(Stamina + FloatToRecover, 0.0f, 100.0f);
}
///
///
////////////////////////////////////////////////////////
///	DAMAGE-SYSTEM	DAMAGE-SYSTEM	DAMAGE-SYSTEM
///	DAMAGE-SYSTEM	DAMAGE-SYSTEM	DAMAGE-SYSTEM
////////////////////////////////////////////////////
///
///
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
///
void APlayerCharacterC::ApplyHealing_Implementation(float HealAmount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.0f, MaxHealth);

	if (MainHUDWidgetInstance)
	{
		MainHUDWidgetInstance->UpdateHealthbar(CurrentHealth, MaxHealth);
	}
}
///
///
////////////////////////////////////////////////////////
///	INTERACT-SYSTEM	INTERACT-SYSTEM	INTERACT-SYSTEM
///	INTERACT-SYSTEM	INTERACT-SYSTEM	INTERACT-SYSTEM
////////////////////////////////////////////////////
///
///
void APlayerCharacterC::Interact(const FInputActionValue& Value)
{	//	get value of bIsInteracting Input
	bool bIsInteracting = Value.Get<bool>();

	//	If input is true, call the interact function
	if (bIsInteracting)
	{
		InteractLineTrace();
	}
}
//	Called on interacted
void APlayerCharacterC::InteractLineTrace()
{
	//	Struct that saves the info about the raycast impact (Actor, Location, Surface Normal...)
	FHitResult Hit;
	//	Raycast start point
	FVector Start = FollowCamera->GetComponentLocation();
	//	Raycast end point
	FVector End = FollowCamera->GetComponentLocation() + FollowCamera->GetForwardVector() * 1000.0f;

	//	Creates a object with parameters for configuring the raycast (what will ignore, if has to detect complex objects...)
	FCollisionQueryParams QueryParams;
	//	Tells the raycast that has to ignore this actor
	QueryParams.AddIgnoredActor(this);
	
	//	Throws the raycast on the world from Start postion to End position using a collision channel (ECC_Pawn) and saves the result on Hit
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, TraceChannelProperty, QueryParams);
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
