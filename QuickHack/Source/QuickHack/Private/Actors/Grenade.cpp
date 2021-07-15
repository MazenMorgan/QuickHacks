// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Grenade.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Grenade_SM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grenade"));
	
}

void AGrenade::AttachGrenadeToSocket(USceneComponent* Parent, FName Bone)
{
	// FAttachmentTransformRules* AttachmentTransformRules = nullptr;
	// AttachmentTransformRules->LocationRule = EAttachmentRule::SnapToTarget;
	// AttachmentTransformRules->RotationRule = EAttachmentRule::SnapToTarget;
	// AttachmentTransformRules->ScaleRule = EAttachmentRule::KeepWorld;
	Grenade_SM->AttachToComponent(Parent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Bone);
	RootComponent = Grenade_SM;
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(FVector(139.998856f,470.000000f, 436.000702f));
	UE_LOG(LogTemp, Warning, TEXT("AGrenade::BeginPlay"));
}
