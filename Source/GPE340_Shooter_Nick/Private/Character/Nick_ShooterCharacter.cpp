// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Nick_ShooterCharacter.h"
#include "Camera/CameraComponent.h"
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

	/* Follow Camera */
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);  // The default end location of the spring arm by socket.
	FollowCamera->bUsePawnControlRotation = false; // Set camera to false so it only follows the boom.

}

void ANick_ShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANick_ShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

