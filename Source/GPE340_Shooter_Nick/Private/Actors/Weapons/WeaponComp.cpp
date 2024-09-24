// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/WeaponComp.h"
#include "Kismet/GameplayStatics.h"
#include "Character/Nick_ShooterCharacter.h"
#include "Character/ShooterCharacterComp.h"


UWeaponComp::UWeaponComp() :
/* Member Initializer List */
AutoFireRate(0.2f)

{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponComp::BeginPlay()
{
	Super::BeginPlay();
		
	/* Bind the Fire Function */
	FireDelegate.BindUObject(this, &UWeaponComp::FireWeapon);
	WeaponTraceDelegate.BindUObject(this, &UWeaponComp::WeaponTrace);
}

/** Perform a trace from the weapon's muzzle to the
 * end hit location of the crosshair to check for obstructing objects.
 */
void UWeaponComp::WeaponTrace()
{
	/*** Line Trace from Gun Barrel ***/
	FHitResult WeaponResults;
	const FVector WeaponTraceStart{ OwningShooterCharacter->GetShooterComp()->GetSocketTransform().GetLocation() };
	const FVector WeaponTraceEnd{ OwningShooterCharacter->GetShooterComp()->GetVaporEndpoint() };
	GetWorld()->LineTraceSingleByChannel(WeaponResults, WeaponTraceStart, WeaponTraceEnd, ECC_Visibility);

	// Check for hit between the weapons muzzle and the VaporEndPoint
	if (WeaponResults.bBlockingHit)
	{
		// Set the endpoint to the object hit location between the muzzle and where the crosshair is aiming.
		OwningShooterCharacter->GetShooterComp()->SetVaporEndpoint(WeaponResults.Location);
	}
	/*** Spawn the impact particles where the blocking hit was ***/
	if (OwningShooterCharacter->GetShooterComp()->ImpactFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OwningShooterCharacter->GetShooterComp()->ImpactFX,
			OwningShooterCharacter->GetShooterComp()->GetVaporEndpoint());
	}
}

void UWeaponComp::StartFireTimer()
{
	if (OwningShooterCharacter && OwningShooterCharacter->GetShooterComp()->GetbShouldFire())
	{
		FireWeapon();
		OwningShooterCharacter->GetShooterComp()->SetbShouldFire(false);
		GetWorld()->GetTimerManager().SetTimer(AutoFireTimer, this, &UWeaponComp::FireTimerReset, AutoFireRate, false);
	}
}

void UWeaponComp::FireTimerReset()
{
	OwningShooterCharacter->GetShooterComp()->SetbShouldFire(true);
	if (OwningShooterCharacter->GetShooterComp()->GetbFireButtonDown())
	{
		StartFireTimer();
	}
}

void UWeaponComp::FireWeapon()
{
	OwningShooterCharacter->GetShooterComp()->SetWeaponSocketTransform();
	/* Play montage associated with firing weapons */
	UAnimInstance* AnimInstance = OwningShooterCharacter->GetMesh()->GetAnimInstance();
		
	if (AnimInstance && OwningShooterCharacter->GetShooterComp()->HipFireMontage)
	{
		AnimInstance->Montage_Play(OwningShooterCharacter->GetShooterComp()->HipFireMontage);
		AnimInstance->Montage_JumpToSection(FName("StartFire"));
		OwningShooterCharacter->GetShooterComp()->OnCrosshairTrace.Execute();
	}
}

void UWeaponComp::ReloadWeapon()
{
	
}

