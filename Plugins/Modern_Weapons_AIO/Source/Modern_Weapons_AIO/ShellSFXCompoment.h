// Copyright SOUNDFX STUDIO © 2023

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SoundFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "ShellSFXCompoment.generated.h"

USTRUCT(BlueprintType)
struct FCasingsSoundsSurface
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Defaults)
		USoundCue* Cue_AmmunitionCasings;

	FCasingsSoundsSurface()
		: Cue_AmmunitionCasings()
	{}
};

UCLASS(Blueprintable, BlueprintType)
class MODERN_WEAPONS_AIO_API UShellSFXCompoment : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShellSFXCompoment();

public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// SOUND CUES
	////////////////////////////////////////////////////////////////////
	// Bullet and Projectiles Casings
	UPROPERTY(EditDefaultsOnly, Category = AmmunitionSounds)
		FCasingsSoundsSurface CasingsSoundsSurface[SurfaceType_Max];


	////////////////////////////////////////////////////////////////////
	// FUNCTIONS
	////////////////////////////////////////////////////////////////////
	// Play Bullet and Projectiles Casings Sounds
	UFUNCTION(BlueprintCallable, Category = SoundsFunctions)
		void Play_AmmunitionCasings(FVector location, float hitValue, EPhysicalSurface surfaceType);

	// Play Bullet and Projectiles Casings Sounds for Multiplayer Game
	UFUNCTION(BlueprintCallable, Category = SoundsFunctionsForMultiplayer)
		void Play_AmmunitionCasings_ForMultiplayer(FVector location, float hitValue, EPhysicalSurface surfaceType);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_Play_AmmunitionCasings(FVector location, float hitValue, EPhysicalSurface surfaceType);
	void Server_Play_AmmunitionCasings_Implementation(FVector location, float hitValue, EPhysicalSurface surfaceType);
	bool Server_Play_AmmunitionCasings_Validate(FVector location, float hitValue, EPhysicalSurface surfaceType);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void Multi_Play_AmmunitionCasings(FVector location, float hitValue, EPhysicalSurface surfaceType);
	void Multi_Play_AmmunitionCasings_Implementation(FVector location, float hitValue, EPhysicalSurface surfaceType);
	bool Multi_Play_AmmunitionCasings_Validate(FVector location, float hitValue, EPhysicalSurface surfaceType);
		
protected:

	// Called when the game starts
	virtual void BeginPlay() override;
};