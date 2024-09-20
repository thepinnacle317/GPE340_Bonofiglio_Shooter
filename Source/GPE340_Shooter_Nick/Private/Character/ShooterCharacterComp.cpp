// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ShooterCharacterComp.h"

#include "Character/InteractionComponent.h"
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
	/* Gun Firing Variables */
	AutoFireRate(0.1f),
	bShouldFire(true),
	bFireButtonDown(false)
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UShooterCharacterComp::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<ACharacter>(GetOwner());

	OnCrosshairTrace.BindUObject(this, &UShooterCharacterComp::CrosshairTrace);

	CurrentCameraFOV = DefaultCameraFOV;
	
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

		WeaponTrace();
		
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

/** Perform a trace from the weapon's muzzle to the
 * end hit location of the crosshair to check for obstructing objects.
 */
void UShooterCharacterComp::WeaponTrace()
{
	/*** Line Trace from Gun Barrel ***/
	FHitResult WeaponResults;
	const FVector WeaponTraceStart{ SocketTransform.GetLocation() };
	const FVector WeaponTraceEnd{ VaporEndPoint };
	GetWorld()->LineTraceSingleByChannel(WeaponResults, WeaponTraceStart, WeaponTraceEnd, ECC_Visibility);

	// Check for hit between the weapons muzzle and the VaporEndPoint
	if (WeaponResults.bBlockingHit)
	{
		// Set the endpoint to the object hit location between the muzzle and where the crosshair is aiming.
		VaporEndPoint = WeaponResults.Location;
	}
	/* Spawn the impact particles where the blocking hit was */
	if (ImpactFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactFX, VaporEndPoint);
	}
}

void UShooterCharacterComp::FirePressed()
{
	bFireButtonDown = true;
	StartFireTimer();
}

void UShooterCharacterComp::FireReleased()
{
	bFireButtonDown = false;
}

void UShooterCharacterComp::FireWeapon()
{
	SetWeaponSocketTransform();
	/* Play montage associated with firing weapons */
	UAnimInstance* AnimInstance = OwningCharacter->GetMesh()->GetAnimInstance();
		
	if (AnimInstance && HipFireMontage)
	{
		AnimInstance->Montage_Play(HipFireMontage);
		AnimInstance->Montage_JumpToSection(FName("StartFire"));CrosshairTrace();
	}
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

void UShooterCharacterComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UShooterCharacterComp::StartFireTimer()
{
	if (bShouldFire)
	{
		FireWeapon();
		bShouldFire = false;
		GetWorld()->GetTimerManager().SetTimer(AutoFireTimer, this, &UShooterCharacterComp::FireTimerReset, AutoFireRate, false);
	}
}

void UShooterCharacterComp::SetWeaponSocketTransform()
{
	const USkeletalMeshSocket* BarrelSocket = OwningCharacter->GetMesh()->GetSocketByName("BarrelSocket");
	if (BarrelSocket)
	{
		SetSocketTransform(BarrelSocket->GetSocketTransform(OwningCharacter->GetMesh()));
	}
}

void UShooterCharacterComp::FireTimerReset()
{
	bShouldFire = true;
	if (bFireButtonDown)
	{
		StartFireTimer();
	}
}

