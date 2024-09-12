// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ShooterAnimInstance.h"
#include "Character/Nick_ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UShooterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	// Redundant try to get a reference to the character in case if fails.
	if (ShooterCharacter == nullptr)
	{
		ShooterCharacter = Cast<ANick_ShooterCharacter>(TryGetPawnOwner());
	}
	if (ShooterCharacter)
	{
		// Get the lateral speed of the character from their velocity
		FVector Velocity{ ShooterCharacter->GetVelocity() };
		// Zero Z so we only get the lateral velocity
		Velocity.Z = 0;
		Speed = Velocity.Size();

		// Check if the character is in the air
		bIsInAir = ShooterCharacter->GetCharacterMovement()->IsFalling();

		// Check if the character is accelerating					Check the magnitude of the vector retrieved from the movement Comp.
		if (ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
		{
			bIsMoving = true;
		}
		else
		{
			bIsMoving = false;
		}
	}
}

void UShooterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	ShooterCharacter = Cast<ANick_ShooterCharacter>(TryGetPawnOwner());
}
