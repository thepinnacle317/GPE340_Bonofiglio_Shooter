// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterCharacterComp.generated.h"

DECLARE_DELEGATE(FCrosshairDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GPE340_SHOOTER_NICK_API UShooterCharacterComp : public UActorComponent
{
	GENERATED_BODY()

public:	

	UShooterCharacterComp();

	UPROPERTY(EditAnywhere, Category="Trace Properties")
	float CrosshairTraceLength = 50000.f;

	UPROPERTY()
	FTransform SocketTransform;
	
	FCrosshairDelegate OnCrosshairTrace;

	/* Directional Dodge Montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DodgeMontage;

	/* Dive Montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> DiveMontage;

protected:

	virtual void BeginPlay() override;
	void CrosshairTrace();

	
	

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat Properties | VFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> VaporTrail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat Properties | VFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> ImpactFX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat Properties | VFX", meta = (AllowPrivateAccess = "true"))
	FVector VaporEndPoint;

	
	

public:
	FORCEINLINE FTransform GetSocketTransform() const { return SocketTransform; }

	
		
};
