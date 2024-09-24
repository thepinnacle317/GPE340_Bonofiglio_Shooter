// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapons/WeaponComp.h"

#include "Actors/Weapons/Weapon_Base.h"
#include "Kismet/GameplayStatics.h"
#include "Character/Nick_ShooterCharacter.h"
#include "Character/ShooterCharacterComp.h"


UWeaponComp::UWeaponComp() :
/* Member Initializer List */
AutoFireRate(0.2f),
ImpulseStrength(10000.f),
WeaponThrowTime(.4f),
bIsFalling(false)

{
	/* Must be true for this component */
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/* Keeps the weapon Upright *** This is used for handling the world widget orientation */
	if (OwningActor->GetItemState() == EItemState::EIS_Falling && bIsFalling)
	{
		const FRotator MeshRotation{0.f, OwningActor->GetItemMesh()->GetComponentRotation().Yaw, 0.f};
		OwningActor->GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);
	}
}

void UWeaponComp::BeginPlay()
{
	Super::BeginPlay();

	/* Get the weapon that owns this component */
	OwningActor = Cast<AWeapon_Base>(GetOwner());
		
	/* Bind the Fire Function */
	FireDelegate.BindUObject(this, &UWeaponComp::FireWeapon);
	WeaponTraceDelegate.BindUObject(this, &UWeaponComp::WeaponTrace);
}

void UWeaponComp::ThrowWeapon()
{
	/* Initialize rotation to the item meshes yaw rotation */
	FRotator MeshRotation{0.f, OwningActor->GetItemMesh()->GetComponentRotation().Yaw, 0.f};
	OwningActor->GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);

	/* Get Mesh Forward Vector */
	const FVector MeshForward{ OwningActor->GetItemMesh()->GetForwardVector() };

	/* Get Mesh Right Vector */
	const FVector MeshRight{ OwningActor->GetItemMesh()->GetRightVector() };
	
	/* Direction to throw the weapon *** -20 degress from the meshes forward vector when called. */
	FVector MeshImpulse = MeshRight.RotateAngleAxis(-20.f, MeshForward);

	/* Give Random Rotation */
	float RandomMeshRotation{FMath::FRandRange(5.f, 15.f)};
	MeshImpulse = MeshRight.RotateAngleAxis(RandomMeshRotation, FVector(0.f,0.f, 1.f));

	/* Impulse Strength */
	MeshImpulse *= 10000.f;
	OwningActor->GetItemMesh()->AddImpulse(MeshImpulse);

	/* Set the bool to falling & start the timer for when it is in the air */
	bIsFalling = true;
	GetWorld()->GetTimerManager().SetTimer(ThrowWeaponHandle, this, &UWeaponComp::StopFalling, WeaponThrowTime);
}

void UWeaponComp::StopFalling()
{
	/* Set the bool since it will stop falling based on the timer and make it ready to be picked up */
	bIsFalling = false;
	OwningActor->SetItemState(EItemState::EIS_PickupReady);
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

