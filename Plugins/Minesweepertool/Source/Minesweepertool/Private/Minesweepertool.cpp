// Copyright Epic Games, Inc. All Rights Reserved.

#include "Minesweepertool.h"

#include "MinesweeperGame.h"
#include "MinesweepertoolStyle.h"
#include "MinesweepertoolCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "Components/UniformGridSlot.h"

static const FName MinesweepertoolTabName("Minesweepertool");

#define LOCTEXT_NAMESPACE "FMinesweepertoolModule"

void FMinesweepertoolModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMinesweepertoolStyle::Initialize();
	FMinesweepertoolStyle::ReloadTextures();

	FMinesweepertoolCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMinesweepertoolCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FMinesweepertoolModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMinesweepertoolModule::RegisterMenus));


	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		"MinesweeperToolTab",
		FOnSpawnTab::CreateRaw(this, &FMinesweepertoolModule::OnSpawnPluginTab)
	)
		.SetDisplayName(FText::FromString("Minesweeper"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

}

void FMinesweepertoolModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMinesweepertoolStyle::Shutdown();

	FMinesweepertoolCommands::Unregister();
}

void FMinesweepertoolModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(
		FName("MinesweeperToolTab")
	);
	
	//FText DialogText = FText::Format(
	//						LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
	//						FText::FromString(TEXT("FMinesweepertoolModule::PluginButtonClicked()")),
	//						FText::FromString(TEXT("Minesweepertool.cpp"))
	//				   );
	//FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FMinesweepertoolModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMinesweepertoolCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMinesweepertoolCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}


TSharedRef<SDockTab> FMinesweepertoolModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	// creatingg game instance here if nul
	if (!Game){ Game = MakeUnique<FMinesweeperGame>();  }

	return SNew(SDockTab).TabRole(ETabRole::NomadTab)  // iam using nomad here because it prvovide docable tooling window, 
		[
			 SNew(SVerticalBox)+ SVerticalBox::Slot().AutoHeight().Padding(10)
			[
					SNew(SHorizontalBox)+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center).Padding(14,4,4,4)
						[
							SNew(STextBlock).Text(FText::FromString(TEXT("Width:")))
						]
						+ SHorizontalBox::Slot().AutoWidth().Padding(4)
						[
							// Setting minimum Value 2 here and maximum limit is 50
							SNew(SSpinBox<int32>).MinValue(2).MaxValue(50).Value(Width)
							.OnValueChanged_Lambda([this](int32 V) { Width = V; })
						]

						+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center).Padding(14, 4, 4, 4)
						[SNew(STextBlock).Text(FText::FromString(TEXT("Height:")))]

						+ SHorizontalBox::Slot().AutoWidth().Padding(4)
						[
							SNew(SSpinBox<int32>)
							.MinValue(2).MaxValue(50)
							.Value(Height)
							.OnValueChanged_Lambda([this](int32 V) { Height = V; })
						]

						+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center).Padding(14, 4, 4, 4)
						[SNew(STextBlock).Text(FText::FromString(TEXT("Bombs:")))]

						+ SHorizontalBox::Slot().AutoWidth().Padding(4)
						[
							SNew(SSpinBox<int32>).MinValue(1).MaxValue(2500).Value(Bombs) // logically max will be(50 * 50) Width, Height Max
							.OnValueChanged_Lambda([this](int32 V) { Bombs = V; })
						]

						+ SHorizontalBox::Slot().AutoWidth().Padding(14, 4, 4, 4)
						[SNew(SButton).Text(FText::FromString(TEXT("Create New Game"))).OnClicked_Lambda([this]()
						{ return OnNewGameClicked(); })
						]
				]

		
			+ SVerticalBox::Slot()
				.FillHeight(1.0f)
				.Padding(10)
				[
					SAssignNew(GridPanel, SUniformGridPanel)
				]
		];

	

	
}

FReply FMinesweepertoolModule::OnNewGameClicked()
{

	if (!GridPanel) return FReply::Handled();

	
	if (!Game){Game = MakeUnique<FMinesweeperGame>();}// creating again game if somhow its become null , just safe check

	const int32 MaxBombs = FMath::Max(0, Width * Height - 1); //max can be 2500
	Bombs = FMath::Clamp(Bombs, 0, MaxBombs);

	Game->Init(Width, Height, Bombs);
	RebuildGridUI();
	
	return FReply::Handled();
}



void FMinesweepertoolModule::RebuildGridUI()
{
	
	TileBorders.Empty();
	TileBorders.SetNum(Width * Height); // same size of grid

	GridPanel->ClearChildren();
	TileButtons.Empty();
	TileTexts.Empty();
	TileButtons.SetNum(Width * Height);
	TileTexts.SetNum(Width * Height);

	for (int32 y = 0; y < Height; ++y)
	{
		for (int32 x = 0; x < Width; ++x)
		{
			const int32 Idx = Index(x, y);

			TSharedPtr<STextBlock> Label;
			TSharedPtr<SButton> Btn;
			TSharedPtr<SBorder> OuterBorder; // this is the one i store and update color of border

			GridPanel->AddSlot(x, y)
			[
		
				SAssignNew(OuterBorder, SBorder)
				.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
				.BorderBackgroundColor(FSlateColor(UnrevealedBorderColor)) // start unrevealed (red)
				.Padding(FMargin(TileBorderThickness)) // thickness of visible border
				[
					// Inner border is transparent (acts as content container)
					SNew(SBorder)
					.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
					.BorderBackgroundColor(FSlateColor(FLinearColor::Transparent))
					.Padding(0)
					[
						
						SAssignNew(Btn, SButton)
      .OnClicked_Lambda([this, x, y]() { return OnTileClicked(x, y); })
      .ContentPadding(FMargin(0))  
[
	 SNew(SBox)
	.HAlign(HAlign_Center)    // center horizontally
	.VAlign(VAlign_Center)    // center vertically
	[
		SAssignNew(Label, STextBlock)
		.Justification(ETextJustify::Center)
		.Text(FText::GetEmpty())
	]
]
						]
					]
				];
						

			// Optional: make button fill nicely
			Btn->SetContentPadding(FMargin(0));
			Btn->SetHAlign(HAlign_Fill);
			Btn->SetVAlign(VAlign_Fill);

			TileButtons[Idx] = Btn;
			TileTexts[Idx]   = Label;
			TileBorders[Idx] = OuterBorder; // store outer border so we can change color later
		}
	}
}


FReply FMinesweepertoolModule::OnTileClicked(int32 X, int32 Y)
{
	if (!Game) return FReply::Handled();

	TArray<FIntPoint> Revealed;
	const bool bHitBomb = Game->Reveal(X, Y, Revealed);

	if (bHitBomb)
	{
		// Reveal all bombs in UI
		for (int32 yy=0; yy<Game->GetHeight(); ++yy)
			for (int32 xx=0; xx<Game->GetWidth();  ++xx)
			{
				const auto& T = Game->Tile(xx, yy);
				if (T.bIsBomb)
				{
					const int32 Idx = Index(xx, yy);
					if (TileTexts.IsValidIndex(Idx) && TileTexts[Idx].IsValid())
					{
						TileTexts[Idx]->SetText(FText::FromString(TEXT("B")));
						
					}
					if (TileButtons.IsValidIndex(Idx) && TileButtons[Idx].IsValid())
					{
						TileButtons[Idx]->SetEnabled(false);
					}
				}}
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Game Over")));
		return FReply::Handled();
	}

	
	// update visuals for all revealed safe tiles
	for (const FIntPoint& P : Revealed)
	{
		UpdateTileVisual(P.X, P.Y);
	}
	return FReply::Handled();
}

void FMinesweepertoolModule::UpdateTileVisual(int32 X, int32 Y)
{
	
	
	if (!Game) return;
	if (!Game->InBounds(X,Y)) return;

	const int32 Idx = Index(X,Y);
	const auto& T = Game->Tile(X,Y);

	if (TileTexts.IsValidIndex(Idx) && TileTexts[Idx].IsValid())
	{
		if (T.Adjacent > 0)
		{
			TileTexts[Idx]->SetText(FText::AsNumber(T.Adjacent));
		}
		else
		{
			TileTexts[Idx]->SetText(FText::GetEmpty());
		}
	}

	if (TileButtons.IsValidIndex(Idx) && TileButtons[Idx].IsValid())
	{
		TileButtons[Idx]->SetEnabled(false);
		//TileButtons[Idx]->SetColorAndOpacity(RevealedColor); 
	}

	if (TileBorders.IsValidIndex(Idx) && TileBorders[Idx].IsValid())
	{
		TileBorders[Idx]->SetBorderBackgroundColor(FSlateColor(RevealedBorderColor));
	}
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMinesweepertoolModule, Minesweepertool)