// Copyright Epic Games, Inc. All Rights Reserved.


#include "MyShootThemUpGameModeBase.h"
#include "STUPlayerController.h"
#include "STUBaseCharacter.h"

AMyShootThemUpGameModeBase::AMyShootThemUpGameModeBase()
{
	DefaultPawnClass = ASTUBaseCharacter::StaticClass();
	PlayerControllerClass = ASTUPlayerController::StaticClass();
}
