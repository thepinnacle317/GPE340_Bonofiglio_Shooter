// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Nick_ShooterPlayerController.generated.h"


/* Forward Declarations */
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class ANick_ShooterCharacter;

/**
 * 
 */
UCLASS()
class GPE340_SHOOTER_NICK_API ANick_ShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

private:

	TObjectPtr<ANick_ShooterCharacter> PossessedCharacter;
	/* Enhanced Input */
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> ShooterIMC;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ShootAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> AimAction;

	/* Movement */
	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void JumpStarted();
	void JumpEnd();

	/* Gameplay Actions */
	void FireWeapon();
	
};
