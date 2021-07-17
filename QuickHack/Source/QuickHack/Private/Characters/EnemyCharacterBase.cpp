// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Abilities/GenericAbilitySystemComponent.h"
#include "Abilities/AttributeSets/GenericAttributeSet.h"
#include "Abilities/GenericGameplayAbility.h"
#include <GameplayEffectTypes.h>
#include "Actors/Grenade.h"

// Sets default values
AEnemyCharacterBase::AEnemyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true; 

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

}

void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	SpawnGrenade();
}

void AEnemyCharacterBase::Die()
{
	Super::Die();
	//TODO@ Die ragdoll
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetSimulatePhysics(true);
}

void AEnemyCharacterBase::SpawnGrenade()
{
	if(GrenadeClass)
	{		
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnedGrenade = GetWorld()->SpawnActor<AGrenade>(GrenadeClass, GetActorLocation(), GetActorRotation(), SpawnParameters);
		if(SpawnedGrenade)
		{
			SpawnedGrenade->AttachGrenadeToSocket(GetMesh(), GrenadeSocketName);
		}
	}
}
