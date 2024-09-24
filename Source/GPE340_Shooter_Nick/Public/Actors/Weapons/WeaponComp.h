// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComp.generated.h"


DECLARE_DELEGATE(FOnFireDelegate);
DECLARE_DELEGATE(FWeaponTraceDelegate);

class ANick_ShooterCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType)
class GPE340_SHOOTER_NICK_API UWeaponComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponComp();

	/* Runs a trace from the barrel socket to the end of the crosshairs trace */
	void WeaponTrace();

	void StartFireTimer();
	
	UFUNCTION()
	void FireTimerReset();

	FOnFireDelegate FireDelegate;
	FWeaponTraceDelegate WeaponTraceDelegate;
	
	/* Used to set the rate of fire for the weapon when the fire input is held down */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter Core | Combat Properties | Weapons")
	float AutoFireRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Shooter Core | Combat Properties")
	TObjectPtr<ANick_ShooterCharacter> OwningShooterCharacter;

	/* Timer For weapon Firing */
	FTimerHandle AutoFireTimer;
	
	void FireWeapon();

	void ReloadWeapon();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE void SetOwningShooterCharacter(ANick_ShooterCharacter* ShooterCharacter) { OwningShooterCharacter = ShooterCharacter; }
};
