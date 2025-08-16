#pragma once
#include "CoreMinimal.h"

struct FMineTile
{
	bool bIsBomb = false;
	bool bRevealed = false;
	int32 Adjacent = 0;
};

class FMinesweeperGame
{
public:
	void Init(int32 InW, int32 InH, int32 InBombs);
	
	// this resturns true if a bomb and OutRevealed have all times that changed to reveal
	bool Reveal(int32 X, int32 Y, TArray<FIntPoint>& OutRevealed);

	int32 GetWidth() const { return Width; }
	int32 GetHeight() const { return Height; }
	const FMineTile& Tile(int32 X, int32 Y) const { return Tiles[Index(X,Y)]; }
	//in bounds saved from invalid index here
	bool InBounds(int32 X, int32 Y) const { return X>=0 && Y>=0 && X<Width && Y<Height; }

private:
	int32 Width = 0, Height = 0, BombCount = 0;
	TArray<FMineTile> Tiles;

	int32 Index(int32 X, int32 Y) const { return Y * Width + X; }
	void ComputeAdjacents();
};
