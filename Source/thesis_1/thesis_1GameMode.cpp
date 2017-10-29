// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "thesis_1GameMode.h"
#include "thesis_1Character.h"
#include "UObject/ConstructorHelpers.h"

Athesis_1GameMode::Athesis_1GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Characters/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
