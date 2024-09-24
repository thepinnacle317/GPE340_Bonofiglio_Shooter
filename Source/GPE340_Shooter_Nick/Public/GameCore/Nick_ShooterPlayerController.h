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

public:

	UPROPERTY(BlueprintReadOnly)
	float Forward_Backward;
	
	UPROPERTY(BlueprintReadOnly)
	float Left_Right;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Possessed Pawn")
	TObjectPtr<ANick_ShooterCharacter> PossessedCharacter;

private:
	
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
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> DodgeAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> DropItemAction;

	/* Movement */
	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void MoveCompleted();
	void JumpStarted();
	void JumpEnd();
	void Dodge();
	FName GetDirectionalDodgeSection();

	/* Gameplay Actions */
	void Firing();
	void NotFiring();
	void AimStarted();
	void AimCompleted();
	void Interact();
	void DropHeldItem();
	
};
