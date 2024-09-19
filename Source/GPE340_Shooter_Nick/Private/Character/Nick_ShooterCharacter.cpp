// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Nick_ShooterCharacter.h"
#include "Camera/CameraComponent.h"
#include "Character/ShooterCharacterComp.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


ANick_ShooterCharacter::ANick_ShooterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create and define the camera boom. (Pulls in toward character when colliding).
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.f; // Camera Follow Distance
	CameraBoom->bUsePawnControlRotation = true; // Rotate the boom based on the controller.

	CameraBoom->SocketOffset = FVector(0.f, 50.f, 70.f);

	/* Follow Camera */
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);  // The default end location of the spring arm by socket.
	FollowCamera->bUsePawnControlRotation = false; // Set camera to false, so it only follows the boom.

	/* Do not rotate the character mesh when the controller rotates */ 
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	/* Rotate the character to player input movement */
	// TODO: This will change when strafe movement is implemented along with combat states.
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = .4f; // Control of the character while in the air.
	
	ShooterCharacterComp = CreateDefaultSubobject<UShooterCharacterComp>(TEXT("Shooter Character Component"));
}

void ANick_ShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Can be substituted for a more generic delegate that can be bound based on actions.
	OnFiredWeapon.BindUObject(this, &ANick_ShooterCharacter::FireWeapon);
	OnAiming.BindUObject(this, &ANick_ShooterCharacter::Aim);
	
	/* Set the Camera FOV to the value assigned in the shooter Comp */
	FollowCamera->FieldOfView = ShooterCharacterComp->DefaultCameraFOV;
	
	/* Set the character movement component to the value assigned in the shooter component */
	GetCharacterMovement()->MaxWalkSpeed = ShooterCharacterComp->DefaultCharacterSpeed;
}

void ANick_ShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* Smoothly switch between the aiming and default FOV */
	InterpolateCameraFOV(DeltaTime);
	
	/* Update the sensitivity rate based on the state of aiming */
	ShooterCharacterComp->SetAimSensitivity();
}

void ANick_ShooterCharacter::SetWeaponSocketTransform() const
{
	const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName("BarrelSocket");
	if (BarrelSocket)
	{
		ShooterCharacterComp->SetSocketTransform(BarrelSocket->GetSocketTransform(GetMesh()));
	}
}

void ANick_ShooterCharacter::FireWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire Weapon"));
	
	SetWeaponSocketTransform();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ShooterCharacterComp->HipFireMontage)
	{
		AnimInstance->Montage_Play(ShooterCharacterComp->HipFireMontage);
		AnimInstance->Montage_JumpToSection(FName("StartFire"));
		ShooterCharacterComp->OnCrosshairTrace.ExecuteIfBound();
	}
}

void ANick_ShooterCharacter::Aim()
{
	// Can decrease size of the reticle as well for more precise targeting.
}

void ANick_ShooterCharacter::InterpolateCameraFOV(float DeltaTime)
{
	if (ShooterCharacterComp->GetbIsAiming())
	{
		/* Aiming : Interpolate to the Aiming FOV value */
		ShooterCharacterComp->SetCurrentCameraFOV(FMath::FInterpTo(ShooterCharacterComp->GetCurrentCameraFOV(),
			ShooterCharacterComp->AimingCameraFOV, DeltaTime, ShooterCharacterComp->AimInterpSpeed));
	}
	else
	{
		/* Not Aiming : Interpolate to the Default FOV value */
		ShooterCharacterComp->SetCurrentCameraFOV(FMath::FInterpTo(ShooterCharacterComp->GetCurrentCameraFOV(),
			ShooterCharacterComp->DefaultCameraFOV, DeltaTime, ShooterCharacterComp->AimInterpSpeed));
	}
	/* Set the Cameras FOV Value */
	GetFollowCamera()->SetFieldOfView(ShooterCharacterComp->GetCurrentCameraFOV());
}

