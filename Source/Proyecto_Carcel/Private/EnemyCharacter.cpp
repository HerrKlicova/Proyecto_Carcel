// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemyFunctionsLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	SetExclamationColor(FLinearColor::Blue);
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//	init current health as max health (100.0f)
	CurrentHealth = MaxHealth;
	StartOffset = GetActorLocation() + StartOffset;
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//	checks if the target actor is near enough to throw the linetraces
	if (UKismetMathLibrary::Vector_Distance2DSquared(GetActorLocation(), TargetActor->GetActorLocation()) < FMath::Square(VisionRadius))
	{
		
		//UEnemyFunctionsLibrary FunctionsLibrary;
		//FHitResult HitObject1 = FunctionsLibrary.ConeTraceByChannel(GetWorld(),StartOffset, NumberOfLines, MaxAngle, VisionRadius, GetActorForwardVector(), GetActorUpVector());

		//	create an object to save the return of the function
		FHitResult HitObject = UEnemyFunctionsLibrary::ConeTraceByChannel(GetWorld(),StartOffset, NumberOfLines, MaxAngle, VisionRadius, GetActorForwardVector(), GetActorUpVector());
		//	if hits an object, will happen things on the actor
		if (HitObject.bBlockingHit)
		{
			if (HitObject.GetActor() == TargetActor)
			{
				SetExclamationColor(FLinearColor::Green);
			}
			else
			{
				SetExclamationColor(FLinearColor::Red);
			}
		}
	}
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyCharacter::ApplyDamage_Implementation(float DamageAmount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);
	
	CurrentHealth -= DamageAmount;

	if (CurrentHealth <= 0)
	{
		Destroy();
	}
}
//	function that changes the color of the exclamation
void AEnemyCharacter::SetExclamationColor (FLinearColor LinearColor)
{
	FName Color = TEXT("SurfaceColor");
	FVector Parameter = UKismetMathLibrary::Conv_LinearColorToVector(LinearColor);
	
	StaticMesh->SetVectorParameterValueOnMaterials(Color, Parameter);
}