// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComp.generated.h"

/* Weapon Delegates */
DECLARE_DELEGATE(FOnFireDelegate);
DECLARE_DELEGATE(FWeaponTraceDelegate);

/* Forward Declarations */
class AWeapon_Base;
class ANick_ShooterCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType)
class GPE340_SHOOTER_NICK_API UWeaponComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponComp();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Runs a trace from the barrel socket to the end of the crosshairs trace */
	void WeaponTrace();

	/* Starts the fire timer for automatic fire */
	void StartFireTimer();
	/* Used to reset the fire timer if we stop or are still holding down the input */
	UFUNCTION()
	void FireTimerReset();

	/* Applies and impulse to the weapon it the forward direction *** Called in DropWeapon() */
	void ThrowWeapon();

	/* Weapon Delegates */
	FOnFireDelegate FireDelegate;
	FWeaponTraceDelegate WeaponTraceDelegate;

	/* Reference to the Characer */  // BAAAD!  This is a workaround for now
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Shooter Core | Combat Properties")
	TObjectPtr<ANick_ShooterCharacter> OwningShooterCharacter;
	
	/* Used to set the rate of fire for the weapon when the fire input is held down */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter Core | Combat Properties | Weapons")
	float AutoFireRate;

	/* Designer variable for how far they want the weapon to fly in front of the character when dropped */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter Core | Combat Properties | Weapons")
	float ImpulseStrength;

	/* Should be scaled based on strength *** Can be used for more bounces or distance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter Core | Combat Properties | Weapons")
	float WeaponThrowTime;

	/* Reference to the actor that owns the component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Shooter Core | Combat Properties")
	TObjectPtr<AWeapon_Base> OwningActor;

	/* Timer For weapon Firing */
	FTimerHandle AutoFireTimer;

	/* Handles firing the weapon */
	void FireWeapon();

	/* Used for reloading the weapon */
	void ReloadWeapon();

protected:
	virtual void BeginPlay() override;

	/* Resets the falling variable and sets the item state to PickUpReady so that the player can retrieve it again */
	void StopFalling();

	// TODO: Create a function and timer for how long the item will live in the world once it has been dropped.  When it expired destroy it.
	// Reset the timer if it has been picked up again.

private:
	/* Handle for controlling and manager the timer for the throw states */
	FTimerHandle ThrowWeaponHandle;
	bool bIsFalling;

public:
	FORCEINLINE void SetOwningShooterCharacter(ANick_ShooterCharacter* ShooterCharacter) { OwningShooterCharacter = ShooterCharacter; }
};
