// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MinesweepertoolStyle.h"

class FMinesweepertoolCommands : public TCommands<FMinesweepertoolCommands>
{
public:

	FMinesweepertoolCommands()
		: TCommands<FMinesweepertoolCommands>(TEXT("Minesweepertool"), NSLOCTEXT("Contexts", "Minesweepertool", "Minesweepertool Plugin"), NAME_None, FMinesweepertoolStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
