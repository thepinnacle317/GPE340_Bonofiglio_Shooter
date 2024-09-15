// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Nick_ShooterCharacter.generated.h"

/* Forward Declarations */
class UCameraComponent;
class USpringArmComponent;
class UAnimMontage;

DECLARE_DELEGATE(FFireDelegate);
DECLARE_DELEGATE(FAimDelegate);

UCLASS()
class GPE340_SHOOTER_NICK_API ANick_ShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ANick_ShooterCharacter();
	
	FFireDelegate OnFiredWeapon;
	FAimDelegate OnAiming;

protected:
	
	virtual void BeginPlay() override;
	
	/* Gameplay Actions */
	void FireWeapon();
	void Aim();

public:	
	
	virtual void Tick(float DeltaTime) override;

	

private:

	/* Camera boom used to position the camera behind the camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/* Camera that follows the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> HipFireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> AimMontage;
	

public:

	/* Returns Camera Boom Subobject */
	FORCEINLINE TObjectPtr<USpringArmComponent> GetCameraBoom() const { return CameraBoom; }

	/* Returns Follow Camera Subobject */
	FORCEINLINE TObjectPtr<UCameraComponent> GetFollowCamera() const { return FollowCamera; }

};
