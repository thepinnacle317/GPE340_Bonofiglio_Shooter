// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Nick_ShooterCharacter.h"
#include "Camera/CameraComponent.h"
#include "Character/ShooterCharacterComp.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ANick_ShooterCharacter::ANick_ShooterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create and define the camera boom. (Pulls in toward character when colliding).
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f; // Camera Follow Distance
	CameraBoom->bUsePawnControlRotation = true; // Rotate the boom based on the controller.

	CameraBoom->SocketOffset = FVector(0.f, 100.f, 50.f);

	/* Follow Camera */
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);  // The default end location of the spring arm by socket.
	FollowCamera->bUsePawnControlRotation = false; // Set camera to false, so it only follows the boom.

	/* Do not rotate the character mesh when the controller rotates */ 
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	/* Rotate the character to player input movement */
	// TODO: This will change when strafe movement is implemented along with combat states.
	GetCharacterMovement()->bOrientRotationToMovement = true;
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
	
	
}

void ANick_ShooterCharacter::SetWeaponSocketTransform()
{
	const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName("BarrelSocket");
	if (BarrelSocket)
	{
		ShooterCharacterComp->SocketTransform = BarrelSocket->GetSocketTransform(GetMesh());
	}
}

void ANick_ShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANick_ShooterCharacter::FireWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire Weapon"));
	SetWeaponSocketTransform();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HipFireMontage)
	{
		AnimInstance->Montage_Play(HipFireMontage);
		AnimInstance->Montage_JumpToSection(FName("StartFire"));
		ShooterCharacterComp->OnCrosshairTrace.ExecuteIfBound();
	}
}

void ANick_ShooterCharacter::Aim()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AimMontage)
	{
		AnimInstance->Montage_Play(AimMontage);
		AnimInstance->Montage_JumpToSection(FName("StartAim"));
	}

	// Do stuff with the reticle and zoom FOV here.
}

