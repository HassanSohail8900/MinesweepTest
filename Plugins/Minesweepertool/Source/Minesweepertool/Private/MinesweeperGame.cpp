#include "MinesweeperGame.h"



void FMinesweeperGame::Init(const int32 InW, const int32 InH, const int32 InBombs)
{

    
    Width  = FMath::Max(2, InW); // here i am just ensuring minimum board size 
    Height = FMath::Max(2, InH);

    const int32 TotalTiles = Width * Height;
    const int32 MaxBombs   = FMath::Max(0, TotalTiles - 1);
    
    BombCount = FMath::Clamp(InBombs, 0, MaxBombs);

    
    Tiles.Reset();  // Just clear previous tiles here and set new empty tiles size
    Tiles.SetNumZeroed(TotalTiles);

    
    TArray<int32> Indices;
    Indices.Reserve(TotalTiles);
    for (int32 i = 0; i < TotalTiles; ++i){
        Indices.Add(i);
    }

    for (int32 i = Indices.Num() - 1; i > 0; --i){
        const int32 j = FMath::RandRange(0, i);
        if (i != j)
        {
            Swap(Indices[i], Indices[j]);
        }
        
    }
    for (int32 b = 0; b < BombCount; ++b)// here i m placing bombs at the first count  in shuffled indices
    {
        Tiles[Indices[b]].bIsBomb = true;
    }

   
    ComputeAdjacents();  // this function compute adjacent numbers for non bomb tiles
}

void FMinesweeperGame::ComputeAdjacents()
  {
    
    static constexpr int32 XOffset[8] = { -1,  0,  1, -1, 1, -1, 0, 1 }; // this is neighbour indexes to check 
    static constexpr int32 YOffset[8] = { -1, -1, -1,  0, 0,  1, 1, 1 };

    const int32 W = Width;
    
    const int32 H = Height;

    for (int32 y = 0; y < H; ++y)
    {
        for (int32 x = 0; x < W; ++x)
        {
            
            FMineTile& Tile = Tiles[Index(x, y)];
            
            if (Tile.bIsBomb)
            {
                
                Tile.Adjacent = 0;
                continue;
                
            }

            int32 NeighborBombs = 0;
            
            //counting neighbour bombs
            for (int32 k = 0; k < 8; ++k){
                
                const int32 netX = x + XOffset[k];
                const int32 netY = y + YOffset[k];

                if (InBounds(netX, netY) && Tiles[Index(netX, netY)].bIsBomb)
                {
                    ++NeighborBombs;
                }
            }
            Tile.Adjacent = NeighborBombs;

        }

    }
  }

bool FMinesweeperGame::Reveal(int32 X, int32 Y, TArray<FIntPoint>& OutRevealed){
    
    OutRevealed.Reset(); //cleaning here

    if (!InBounds(X, Y))
    {
        return false;
    }

    FMineTile& StartTile = Tiles[Index(X, Y)];
    if (StartTile.bRevealed)
    {
        return false;
    }

    
    if (StartTile.bIsBomb)  // => If first click is a bom reveal it and indicate bomb hit
        {
        StartTile.bRevealed = true;
        OutRevealed.Add(FIntPoint(X, Y));
        return true;
    }

    
    TArray<FIntPoint> Queue;   // usign int32 here
    
    Queue.Reserve(128); //small reserve to reduces few reallocs on typical boards
    Queue.Add(FIntPoint(X, Y));

    // now reserving here maximum possible to avoid repeated reallocations when ading revealed tiles
    
    OutRevealed.Reserve(FMath::Min(Width * Height, 256));

    int32 Head = 0;
    while (Head < Queue.Num())
    {
        const FIntPoint Current = Queue[Head++];
        FMineTile& Tile = Tiles[Index(Current.X, Current.Y)];

        if (Tile.bRevealed){continue;}
        
        Tile.bRevealed = true;
        OutRevealed.Add(Current);
        
        if (Tile.Adjacent == 0) {
            for (int32 ny = Current.Y - 1; ny <= Current.Y + 1; ++ny)
            {
                for (int32 nx = Current.X - 1; nx <= Current.X + 1; ++nx)
                {
                    // skip center and out of bounds positions
                    if ((nx == Current.X && ny == Current.Y) || !InBounds(nx, ny))
                    {
                        continue;
                    }

                    FMineTile& Neighbor = Tiles[Index(nx, ny)];
                    if (!Neighbor.bRevealed && !Neighbor.bIsBomb)
                    {
                        Queue.Add(FIntPoint(nx, ny));
                    }
                }

            }
           }
       }

    return false;

    
}
