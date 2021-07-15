// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"
#include <GameplayEffectTypes.h>

#include "Characters/CharacterBase.h"

#include "QuickHackCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;
class UUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuickHackWidgetVisibilityChanged, bool, bVisible);

UCLASS(config=Game)
class AQuickHackCharacter : public ACharacterBase
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* FP_MuzzleLocation;

	/** Gun mesh: VR view (attached to the VR controller directly, no arm, just the actual gun) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* VR_Gun;

	/** Location on VR gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* VR_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	
public:
	AQuickHackCharacter();
	//------------------------------------------------------

	virtual void Tick(float DeltaSeconds) override;
	
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AQuickHackProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	UPROPERTY(BlueprintAssignable, Category="QuickHackCharacter|UI")
	FOnQuickHackWidgetVisibilityChanged OnQuickHackWidgetVisibilityChanged;
protected:

	FPostProcessSettings DefaultPostProcessSettings;

	UPROPERTY(EditDefaultsOnly, Category="QuickHackCharacter|PostProcess")
	FPostProcessSettings OnTabPressedPostProcessSettings;

	UPROPERTY(BlueprintReadOnly, Category="QuickHackCharacter|Time")
	float LastActiveTime = 0.0f;
	bool bQuickHackWidgetIsInactive;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="QuickHackCharacter|Time", meta=(UIMin = 0.0f, UIMax = 1.0f))
	float SlowMotionTimeDilation = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, Category = "QuickHackCharacter|UI")
	TSubclassOf<UUserWidget> QuickHackHUDClass;
	UPROPERTY(BlueprintReadOnly, Category = "QuickHackCharacter|UI")
	UUserWidget* QuickHackWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "QuickHackCharacter|UI")
	TSubclassOf<UUserWidget> DefaultHUDClass;
	UPROPERTY(BlueprintReadOnly, Category = "QuickHackCharacter|UI")
	UUserWidget* DefaultHUDWidget;

	UPROPERTY(BlueprintReadOnly, Category="QuickHackCharacter")
	bool bStartLineTrace = false;

	virtual void PreInitializeComponents() override;
	virtual void BeginPlay();

	UFUNCTION(BlueprintImplementableEvent, Category="QuickHackCharacter", meta = (DisplayName = "SetDefaultUIValues"))
	void BP_SetDefaultUIValues();
	
	void OnFire();
	void MoveForward(float Val);
	void MoveRight(float Val);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	void OnTabPressed();
	void OnTabReleased();
	
};

