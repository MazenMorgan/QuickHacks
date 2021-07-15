// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"
#include <GameplayEffectTypes.h>

#include "CharacterBase.h"


#include "EnemyCharacterBase.generated.h"

UCLASS()
class QUICKHACK_API AEnemyCharacterBase : public ACharacterBase
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
public:
	AEnemyCharacterBase();

	public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	
	virtual void BeginPlay() override;
	virtual void Die() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="EnemyCharacter|Grenade")
	TSubclassOf<class AGrenade> GrenadeClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="EnemyCharacter|Grenade")
	FName GrenadeSocketName;
	UPROPERTY(BlueprintReadOnly, Category="EnemyCharacter|Grenade")
	class AGrenade* SpawnedGrenade;
	UFUNCTION(BlueprintCallable)
	void SpawnGrenade();
};
