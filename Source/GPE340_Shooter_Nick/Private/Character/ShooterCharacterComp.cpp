// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ShooterCharacterComp.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values for this component's properties
UShooterCharacterComp::UShooterCharacterComp() :
	/* Initialize Values : Put these in order by initialization */
	DefaultCameraFOV(90.f),
	AimingCameraFOV(40.f),
	bIsAiming(false),
	CurrentCameraFOV(0.f),
	AimInterpSpeed(20.f)
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UShooterCharacterComp::BeginPlay()
{
	Super::BeginPlay();

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

