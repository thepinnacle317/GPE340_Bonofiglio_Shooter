// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ShooterCharacterComp.h"
#include "Actors/Weapons/WeaponComp.h"
#include "Actors/Weapons/Weapon_Base.h"
#include "Character/Nick_ShooterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values for this component's properties
UShooterCharacterComp::UShooterCharacterComp() :
/* Member Initializer List : Put these in order by initialization */
	/* Camera FOV Values */
	DefaultCameraFOV(90.f),
	AimingCameraFOV(40.f),
	AimInterpSpeed(20.f),
	/* Character Speed Values */
	DefaultCharacterSpeed(500.f),
	AimingCharacterSpeed(350),
	bIsAiming(false),
	CurrentCameraFOV(0.f),
	/* Aim Sensitivity */
	BaseLookUpRate(45.f),
	BaseTurnRate(45.f),
	AtReadyTurnRate(90.f),
	AtReadyLookUpRate(90.f),
	AimingTurnRate(25.f),
	AimingLookUpRate(25.f),
	/* Gun Input Variables */
	bFireButtonDown(false),
	bShouldFire(true)
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UShooterCharacterComp::BeginPlay()
{
	Super::BeginPlay();

	/* Character reference for the owner of the component *** Used mostly for getting skeletal data */
	OwningCharacter = Cast<ANick_ShooterCharacter>(GetOwner());

	/* Crosshair Trace delegate binding */
	OnCrosshairTrace.BindUObject(this, &UShooterCharacterComp::CrosshairTrace);

	/* Base setting of the Camera FOV */
	CurrentCameraFOV = DefaultCameraFOV;

	/* Spawn & Equip the weapon for the character at the start of play */
	EquipWeapon(SpawnDefaultWeapon());
	
}

void UShooterCharacterComp::CrosshairTrace()
{
	// Get the current size of the viewport
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	// Get the center of the screen.
	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	
	CrosshairLocation.Y -= 50.f; // Move the crosshair up 50 units on the screen.
	
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	// Transforms the given 2D screen space coordinate into a 3D world-space point and direction.
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(GetOwner(), 0),
		CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);

	// Was the deprojection successful
	if (bScreenToWorld)
	{
		FHitResult ScreenTraceHit;
		const FVector Start{CrosshairWorldPosition};
		const FVector End{CrosshairWorldPosition + CrosshairWorldDirection * CrosshairTraceLength};

		// Set Beam end location for particle effects if no hit.
		VaporEndPoint = End;

		// Perform Line Trace From Crosshair.
		GetWorld()->LineTraceSingleByChannel(ScreenTraceHit, Start, End, ECC_Visibility);

		/* Spawn Impact Particles */
		if (ScreenTraceHit.bBlockingHit)
		{
			/* End the beam particle where the blocking hit was */
			VaporEndPoint = ScreenTraceHit.Location;
		}

		if (CurrentWeapon != nullptr)
		{
			// Call WeaponTrace Delegate
			CurrentWeapon->WeaponComponent->WeaponTraceDelegate.Execute();
		}
		
		
		/* Spawn the Beam Vapor Trail */
		if (VaporTrail)
		{
			UParticleSystemComponent* VaporBeam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), VaporTrail, SocketTransform);
			if (VaporBeam)
			{
				// End the beam based on the vector and parameter value.
				VaporBeam->SetVectorParameter(FName("Target"), VaporEndPoint);
			}
		}
	}
}

AWeapon_Base* UShooterCharacterComp::SpawnDefaultWeapon()
{
	/* Make sure that the weapon class was set in the character and is valid */
	if (DefaultWeaponClass)
	{
		/* Set the local WeaponActor to the spawn class, so it can be attached */
		return GetWorld()->SpawnActor<AWeapon_Base>(DefaultWeaponClass);
	}
	return nullptr;
}

void UShooterCharacterComp::EquipWeapon(AWeapon_Base* WeaponToBeEquipped)
{
	if (WeaponToBeEquipped)
	{
		/* Get the socket that the weapon will be attached to */
		const USkeletalMeshSocket* WeaponSocket = OwningCharacter->GetMesh()->GetSocketByName(FName("WeaponSocket_R"));

		if (WeaponSocket)
		{
			/* Attach the weapon to the weapon socket on the right hand */
			WeaponSocket->AttachActor(WeaponToBeEquipped, OwningCharacter->GetMesh());
		}

		/* Sets the Current Weapon to the weapon that was spawned */
		CurrentWeapon = WeaponToBeEquipped;

		CurrentWeapon->OwningCharacter = OwningCharacter;
		CurrentWeapon->WeaponComponent->OwningShooterCharacter = OwningCharacter;

		/* Change the state of the Item/Weapon to equipped*/
		/* Set Collision Box to ignore all collision channels, so we do not hit it or trigger the widget Based on the state*/
		CurrentWeapon->SetItemState(EItemState::EIS_Equipped);
	}
}

void UShooterCharacterComp::FirePressed()
{
	bFireButtonDown = true;
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->WeaponComponent->StartFireTimer();
	}
}

void UShooterCharacterComp::FireReleased()
{
	bFireButtonDown = false;
}

void UShooterCharacterComp::SetAimSensitivity()
{
	if (bIsAiming)
	{
		/* Set the camera to a lower sensitivity with the higher camera zoom */
		BaseTurnRate = AimingTurnRate;
		BaseLookUpRate = AimingLookUpRate;
	}
	else
	{
		/* Set the rates to higher sensitivity when the camera is not zoomed in */
		BaseTurnRate = AtReadyTurnRate;
		BaseLookUpRate = AtReadyLookUpRate;
	}
}

void UShooterCharacterComp::SetbIsAiming(bool Value)
{
	bIsAiming = Value;
}

void UShooterCharacterComp::SetSocketTransform(const FTransform& Value)
{
	SocketTransform = Value;
}

void UShooterCharacterComp::SetCurrentCameraFOV(float Value)
{
	CurrentCameraFOV = Value;
}

void UShooterCharacterComp::SetWeaponSocketTransform()
{
	const USkeletalMeshSocket* BarrelSocket = OwningCharacter->GetMesh()->GetSocketByName("BarrelSocket");
	if (BarrelSocket)
	{
		SetSocketTransform(BarrelSocket->GetSocketTransform(OwningCharacter->GetMesh()));
	}
}

