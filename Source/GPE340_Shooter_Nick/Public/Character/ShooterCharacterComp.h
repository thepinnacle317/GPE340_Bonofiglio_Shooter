// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterCharacterComp.generated.h"

DECLARE_DELEGATE(FCrosshairDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType)
class GPE340_SHOOTER_NICK_API UShooterCharacterComp : public UActorComponent
{
	GENERATED_BODY()

public:	

	UShooterCharacterComp();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/* Used to set the trace range for the crosshairs */
	UPROPERTY(EditAnywhere, Category="Shooter Core | Camera Properties | Trace Properties")
	float CrosshairTraceLength = 50000.f;

	/* Method used for setting the value of aiming in the controller */
	void SetbIsAiming(bool Value);
	
	/* Used to set the transform of the weapon barrel socket. This is called in Nick_ShooterCharacter */
	void SetSocketTransform(const FTransform& Value);

	/* * * Delegate Handles * * */
	FCrosshairDelegate OnCrosshairTrace;

	/* Directional Dodge Montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter Core | Movement")
	TObjectPtr<UAnimMontage> DodgeMontage;

	/* Dive Montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter Core | Movement")
	TObjectPtr<UAnimMontage> DiveMontage;

	/* Aim Montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter Core | Movement")
	TObjectPtr<UAnimMontage> AimMontage;

	/* Bullet Trace particle effects */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter Core | Combat Properties | VFX")
	TObjectPtr<UParticleSystem> VaporTrail;

	/* Bullet Impact visual effects */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter Core | Combat Properties | VFX")
	TObjectPtr<UParticleSystem> ImpactFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter Core | Camera Properties | FOV")
	float DefaultCameraFOV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter Core | Camera Properties | FOV")
	float AimingCameraFOV;

protected:
	virtual void BeginPlay() override;

	/* Line Traces */
	void CrosshairTrace();
	void WeaponTrace();

private:
	/* Location of the bullet vapor particle endpoint */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Shooter Core | Combat Properties | No Edit", meta = (AllowPrivateAccess = "true"))
	FVector VaporEndPoint;

	/* Used to toggle the aiming state */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Shooter Core | Combat Properties | No Edit", meta = (AllowPrivateAccess = "true"))
	bool bIsAiming;

	/* This is set in the character through SetWeaponSocketTransform for the barrelsocket location.
	 * TODO: Update once there is a pickup and item system. */
	UPROPERTY()
	FTransform SocketTransform;
	

public:
	/* The compiler will ultimately determine if these getters will be Inline or not */
	FORCEINLINE FTransform GetSocketTransform() const { return SocketTransform; }
	FORCEINLINE bool GetbIsAiming() const { return bIsAiming; }

	
		
};
