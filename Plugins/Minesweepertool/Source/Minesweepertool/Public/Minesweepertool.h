// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "Widgets/SWidget.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SUniformGridPanel.h"


class FMinesweeperGame;


class FToolBarBuilder;
class FMenuBuilder;


class FMinesweepertoolModule : public IModuleInterface 
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();
	

	// Mineseeper functions
	TSharedRef<SDockTab> OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs);

private:


	TSharedPtr<class FUICommandList> PluginCommands;
	TSharedPtr<SBox> GameAreaBox;
	TArray<TSharedPtr<SBorder>> TileBorders;
	// SLAT Gird panel here
	TSharedPtr<SUniformGridPanel> GridPanel;
	// i m  keeping arrays to update tiles after clicks
	TArray<TSharedPtr<SButton>> TileButtons;
	TArray<TSharedPtr<STextBlock>> TileTexts;
	// Game logic
	TUniquePtr<FMinesweeperGame> Game;

	// UI state
	int32 Width = 8;
	int32 Height = 8;
	int32 Bombs = 10;


	// border colors for tiles
	const float TileBorderThickness = 4.0f;
	 const FLinearColor UnrevealedBorderColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f); // White
	 const FLinearColor RevealedBorderColor   = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f); // Red



	 void RegisterMenus();
    FReply OnNewGameClicked();
    FReply OnTileClicked(int32 X, int32 Y);
    void RebuildGridUI();                 // rebuild buttons for current W/H
    void UpdateTileVisual(int32 X, int32 Y);
    int32 Index(int32 X, int32 Y) const { return Y * Width + X; }

};
