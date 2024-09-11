// Copyright Epic Games, Inc. All Rights Reserved.

#include "GPE340_Shooter_NickGameMode.h"
#include "GPE340_Shooter_NickCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGPE340_Shooter_NickGameMode::AGPE340_Shooter_NickGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
