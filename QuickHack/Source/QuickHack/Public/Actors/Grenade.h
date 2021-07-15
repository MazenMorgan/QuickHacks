// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "Grenade.generated.h"

UCLASS()
class QUICKHACK_API AGrenade : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grenade", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Grenade_SM = nullptr;
	
public:	
	// Sets default values for this actor's properties
	AGrenade();

	void AttachGrenadeToSocket(class USceneComponent* Parent, FName Bone);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (ExposeOnSpawn = true))
    float ExploadRange;
    
    UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
    FGameplayEffectSpecHandle DamageEffectSpecHandle;

};
