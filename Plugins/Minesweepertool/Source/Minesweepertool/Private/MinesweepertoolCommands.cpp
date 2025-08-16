// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweepertoolCommands.h"

#define LOCTEXT_NAMESPACE "FMinesweepertoolModule"

void FMinesweepertoolCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "Minesweepertool", "Execute Minesweepertool action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
