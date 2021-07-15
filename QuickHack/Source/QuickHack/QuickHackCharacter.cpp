// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuickHackCharacter.h"
#include "QuickHackProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "Abilities/GenericAbilitySystemComponent.h"
#include "Abilities/AttributeSets/GenericAttributeSet.h"
#include "Abilities/GenericGameplayAbility.h"
#include <GameplayEffectTypes.h>
#include "Kismet/GameplayStatics.h"

#include "Blueprint/UserWidget.h"
#include "Framework/PlayerControllerBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AQuickHackCharacter

AQuickHackCharacter::AQuickHackCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);	
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

}

void AQuickHackCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(bQuickHackWidgetIsInactive)
	{
		LastActiveTime+=DeltaSeconds;
	}
}

void AQuickHackCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UE_LOG(LogTemp, Warning, TEXT("AQuickHackCharacter::PreInitializeComponents"));


}

void AQuickHackCharacter::BeginPlay()
{
	DefaultPostProcessSettings = FirstPersonCameraComponent->PostProcessSettings;

	Mesh1P->SetHiddenInGame(false, true);
	APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0)); 
	if(PlayerController)
	{
		if (QuickHackHUDClass)
		{
			QuickHackWidget = CreateWidget<UUserWidget>(PlayerController, QuickHackHUDClass); // Create Widget
			if (!QuickHackWidget)
			{
				UE_LOG(LogTemp, Warning, TEXT("QuickHackWidget nullptr "));
				return;
			}
			QuickHackWidget->AddToViewport();
			QuickHackWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		if (DefaultHUDClass)
		{
			DefaultHUDWidget = CreateWidget<UUserWidget>(PlayerController, DefaultHUDClass); // Create Widget
			if (!DefaultHUDWidget)
			{
				UE_LOG(LogTemp, Warning, TEXT("Default HUD nullptr "));
				return;
			}
			DefaultHUDWidget->AddToViewport();
			DefaultHUDWidget->SetVisibility(ESlateVisibility::Visible);
		}
		BP_SetDefaultUIValues();
	}

	UE_LOG(LogTemp, Warning, TEXT("AQuickHackCharacter::BeginPlay"));
	Super::BeginPlay();
}

void AQuickHackCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("QuickHackSelect", IE_Pressed, this, &AQuickHackCharacter::OnTabPressed);
	PlayerInputComponent->BindAction("QuickHackSelect", IE_Released, this, &AQuickHackCharacter::OnTabReleased);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AQuickHackCharacter::OnFire);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AQuickHackCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AQuickHackCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AQuickHackCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AQuickHackCharacter::LookUpAtRate);
	
}


void AQuickHackCharacter::OnTabPressed()
{
	FirstPersonCameraComponent->PostProcessSettings = OnTabPressedPostProcessSettings;
	
	bStartLineTrace = true;
	OnQuickHackWidgetVisibilityChanged.Broadcast(true);
	if(DefaultHUDWidget)
	{
		DefaultHUDWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if(QuickHackWidget)
	{
		QuickHackWidget->SetVisibility(ESlateVisibility::Visible);
	}
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), SlowMotionTimeDilation);

	bQuickHackWidgetIsInactive = false;
	LastActiveTime = 0.0f;
}


void AQuickHackCharacter::OnTabReleased()
{
	FirstPersonCameraComponent->PostProcessSettings = DefaultPostProcessSettings;
	bStartLineTrace = false;
	bQuickHackWidgetIsInactive = true;
	if(QuickHackWidget)
	{
		QuickHackWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if(DefaultHUDWidget)
	{
		DefaultHUDWidget->SetVisibility(ESlateVisibility::Visible);
	}
	OnQuickHackWidgetVisibilityChanged.Broadcast(false);
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}


void AQuickHackCharacter::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				World->SpawnActor<AQuickHackProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
		}
	}

	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}
void AQuickHackCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}


void AQuickHackCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}


void AQuickHackCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}


void AQuickHackCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}