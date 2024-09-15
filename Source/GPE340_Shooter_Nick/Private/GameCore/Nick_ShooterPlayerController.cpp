// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/Nick_ShooterPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/Nick_ShooterCharacter.h"
#include "Character/ShooterCharacterComp.h"
#include "GameFramework/CharacterMovementComponent.h"


void ANick_ShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(ShooterIMC, 0);
	}
}

void ANick_ShooterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* ShooterInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	UE_LOG(LogTemp, Warning, TEXT("Input Component Setup."));
	ShooterInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	ShooterInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ThisClass::MoveCompleted);
	ShooterInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	ShooterInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::JumpStarted);
	ShooterInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::JumpEnd);
	ShooterInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ThisClass::Fire);
	ShooterInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &ThisClass::Aim);
	ShooterInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ThisClass::Dodge);

}

void ANick_ShooterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	PossessedCharacter = Cast<ANick_ShooterCharacter>(InPawn);
}

void ANick_ShooterPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// input is a Vector2D
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// find out which way is forward
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		Forward_Backward = MovementVector.Y;
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
		Left_Right = MovementVector.X;
	}
}

void ANick_ShooterPlayerController::MoveCompleted()
{
	Forward_Backward = 0.f;
	Left_Right = 0.f;
}

void ANick_ShooterPlayerController::Look(const FInputActionValue& InputActionValue)
{
	// input is a Vector2D
	FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// add yaw and pitch input to controller
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ANick_ShooterPlayerController::JumpStarted()
{
	if (PossessedCharacter != nullptr)
	{
		PossessedCharacter->Jump();
	}
}

void ANick_ShooterPlayerController::JumpEnd()
{
	if (PossessedCharacter != nullptr)
	{
		PossessedCharacter->StopJumping();
	}
}

void ANick_ShooterPlayerController::Dodge()
{
	/* Get the anim instance from the possesesed character so we can play a montage */
	UAnimInstance* AnimInstance = PossessedCharacter->GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		UAnimMontage* SelectedDodge;
		if (PossessedCharacter->GetCharacterMovement()->bOrientRotationToMovement)
		{
			SelectedDodge = PossessedCharacter->GetShooterComp()->DiveMontage;				
		}
		else
		{
			SelectedDodge = PossessedCharacter->GetShooterComp()->DodgeMontage;
		}
		
		AnimInstance->Montage_Play(SelectedDodge);
		// Jump to the section of the montage that will be determined by input value and switch to the correct case.
		if (!PossessedCharacter->GetCharacterMovement()->bOrientRotationToMovement)
		{
			AnimInstance->Montage_JumpToSection(GetDirectionalDodgeSection());
		}
	}
}

FName ANick_ShooterPlayerController::GetDirectionalDodgeSection()
{
	FName MontageSection;
	int32 SelectIndex = FMath::TruncToInt(Forward_Backward * 2.f + Left_Right) + 2;
	switch (SelectIndex)
	{
	case 0:
		MontageSection = FName("BWD");
		break;
	case 1:
		MontageSection = FName("LEFT");
		break;
	case 2:
		MontageSection = FName("FWD");
		break;
	case 3:
		MontageSection = FName("RIGHT");
		break;
	case 4:
		MontageSection = FName("FWD");
		break;
	}
	return MontageSection;
}


void ANick_ShooterPlayerController::Fire()
{
	PossessedCharacter->OnFiredWeapon.ExecuteIfBound();
}

void ANick_ShooterPlayerController::Aim()
{
	PossessedCharacter->OnAiming.ExecuteIfBound();
	// Call delegate from character to make these changes.
	UE_LOG(LogTemp, Warning, TEXT("Aiming"));
	// TODO: Change FOV of Camera
	// TODO: Play animation associated with the equipped weapon.  ie raise weapon (at ready state).
	// Can decrease size of the reticle as well for more precise targeting.
}

void ANick_ShooterPlayerController::Interact()
{
}
