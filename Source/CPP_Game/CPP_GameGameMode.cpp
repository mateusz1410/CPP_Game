// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPP_GameGameMode.h"
#include "CPP_GameCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACPP_GameGameMode::ACPP_GameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
