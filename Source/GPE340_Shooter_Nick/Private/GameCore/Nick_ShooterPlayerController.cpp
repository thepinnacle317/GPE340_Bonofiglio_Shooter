// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/Nick_ShooterPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"


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
	ShooterInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANick_ShooterPlayerController::Move);
	ShooterInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANick_ShooterPlayerController::Look);
	ShooterInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::JumpStarted);
	ShooterInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::JumpEnd);

}

void ANick_ShooterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	PossessedCharacter = Cast<ACharacter>(InPawn);
}

void ANick_ShooterPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// input is a Vector2D
	FVector2D MovementVector = InputActionValue.Get<FVector2D>();

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
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
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
