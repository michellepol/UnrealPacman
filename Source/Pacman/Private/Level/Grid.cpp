#include "Level/Grid.h"

#include <vector>

#include "Editor.h"
#include "Engine/Blueprint.h"
#include "Engine/World.h"
#include "NavigationSystem.h"
#include "UObject/ObjectMacros.h"

#include "Level/Tile.h"

DEFINE_LOG_CATEGORY(LogGrid);

namespace {

bool IsGridActorPlaced(AGrid *Grid, const TArray<AActor *> &Actors) {
  for (AActor *Actor : Actors) {
    if (Actor == Grid) {
      return true;
    }
  }

  return false;
}

void DeleteRelatedActors(TArray<AActor *> Actors) {
  UE_LOG(LogGrid, Display, TEXT("Delete related actors: %i"), Actors.Num());

  for (AActor *Actor : Actors) {
    if (Actor) {
      Actor->Destroy();
    }
  }
}

bool DetectCollision(const int row, const int col,
                     const std::vector<std::vector<int>> &grid) {
  if (grid.empty() || col < 0 || col >= grid[0].size() || row < 0 || row >= grid.size() ||
      grid[row][col] == 1) {
    return true;
  }
  return false;
}

} // namespace

AGrid::AGrid() {
  FEditorDelegates::OnNewActorsPlaced.AddUObject(this, &AGrid::OnEditorPlaced);
}

void AGrid::Destroyed() {
  Super::Destroyed();

  TArray<AActor *> AttachedActors;
  GetAttachedActors(AttachedActors);

  DeleteRelatedActors(AttachedActors);
}

void AGrid::BeginPlay() {
  Super::BeginPlay();

  TArray<AActor *> AttachedActors;
  GetAttachedActors(AttachedActors);

  for (AActor *Actor : AttachedActors) {
    ATile *Tile = Cast<ATile>(Actor);
    if (Tile == nullptr) {
      continue;
    }

    FVector Location = Actor->GetActorLocation();

    FGridPosition Position = GetTileGridPosition(Location);

    UE_LOG(LogGrid, Display, TEXT("Add to grid row %i, col %i"), Position.row,
           Position.col);

    Tile->SetGridPosition(Position);

    GridTilesIndex.Add(Position, Tile);
  }

  GridInt = ToIntGrid();
}

FGridPosition AGrid::GetTileGridPosition(const FVector WorldLocation) const {
  FVector GridLocation = this->GetActorLocation();
  const int row = (WorldLocation.Y - GridLocation.Y) / TileSize;
  const int col = (WorldLocation.X - GridLocation.X) / TileSize;

  return FGridPosition(row, col);
}

ATile *AGrid::GetTileByLocation(const FVector WorldLocation) const {
  FGridPosition GridPosition = GetTileGridPosition(WorldLocation);

  return GetTileByGridPosition(GridPosition);
}

ATile *AGrid::GetTileByGridPosition(const FGridPosition GridPosition) const {
  ATile *const *Value = GridTilesIndex.Find(GridPosition);

  if (Value == nullptr) {
    UE_LOG(LogGrid, Error, TEXT("Not found value by row %i, col %i"),
           GridPosition.row, GridPosition.col);
    return nullptr;
  }

  ATile *Tile = *Value;

  if (Tile == nullptr) {
    UE_LOG(LogGrid, Error, TEXT("Can't cast actor to tile at row %i, col %i"),
           GridPosition.row, GridPosition.col);
    return nullptr;
  }

  return Tile;
}

bool AGrid::IsCrossRoad(const ATile *Tile,
                        const FAdjacentTiles &AdjacentTiles) {
  int TilesIsNotWall = 0;

  // up
  if (AdjacentTiles.Up && AdjacentTiles.Up->TileType == ETileType::Floor) {
    TilesIsNotWall += 1;
  }

  // right
  if (AdjacentTiles.Right && AdjacentTiles.Right->TileType == ETileType::Floor) {
    TilesIsNotWall += 1;
  }

  // left
  if (AdjacentTiles.Left && AdjacentTiles.Left->TileType == ETileType::Floor) {
    TilesIsNotWall += 1;
  }

  // down
  if (AdjacentTiles.Down && AdjacentTiles.Down->TileType == ETileType::Floor) {
    TilesIsNotWall += 1;
  }
  

  // There is 2 floor tiles
  // =============
  //      x
  // =============

  // There is 3 floor tiles
  // ===== =======
  //      x
  // =============
  return TilesIsNotWall > 2;
}

UFUNCTION(BlueprintCallable)
FAdjacentTiles AGrid::GetAdjacentTiles(const ATile *Tile) const {
  FGridPosition GridPosition = Tile->GetGridPosition();
  
  FAdjacentTiles AdjacentTiles;

  FGridPosition UpPosition =
      FGridPosition(GridPosition.row + 1, GridPosition.col);
  auto *UpTile = GridTilesIndex.Find(UpPosition);
  if(UpTile) {
    AdjacentTiles.Up = *UpTile;
  }

  FGridPosition RightPosition =
      FGridPosition(GridPosition.row, GridPosition.col + 1);
  auto *RightTile = GridTilesIndex.Find(RightPosition);
  if(RightTile) {
    AdjacentTiles.Right = *RightTile;
  }

  FGridPosition LeftPosition =
      FGridPosition(GridPosition.row, GridPosition.col - 1);
  auto *LeftTile = GridTilesIndex.Find(LeftPosition);
  if(LeftTile) {
    AdjacentTiles.Left = *LeftTile;
  }

  FGridPosition DownPosition =
      FGridPosition(GridPosition.row - 1, GridPosition.col);
  auto *DownTile = GridTilesIndex.Find(DownPosition);
  if(DownTile) {
    AdjacentTiles.Down = *DownTile;
  }

  return AdjacentTiles;
}

TArray<TArray<int>> AGrid::ToIntGrid() {
  if (GridTilesIndex.IsEmpty()) {
    UE_LOG(LogGrid, Error, TEXT("GridTilesIndex is empty"));
    return {};
  }

  TArray<TArray<int>> result;
  result.Reserve(Width);

  for (int row = 0; row < Width; row++) {
    TArray<int> int_row;
    int_row.Init(0, Length);
    for (int col = 0; col < Length; col++) {

      ATile **IndexValue =
          GridTilesIndex.Find(FGridPosition{.row = row, .col = col});

      if (!IndexValue) {
        UE_LOG(LogGrid, Error, TEXT("No Tile in index"));
        continue;
      }

      ATile *Tile = *IndexValue;

      if (Tile->TileType == ETileType::Wall) {
        int_row[col] = 1;
      } else {
        int_row[col] = 0;
      }
    }
    result.Add(int_row);
  }

  return result;
}

const TArray<TArray<int>> &AGrid::GetIntGrid() const { return GridInt; }

void AGrid::OnEditorPlaced(UObject *Object, const TArray<AActor *> &Actors) {
  if (!IsGridActorPlaced(this, Actors)) {
    return;
  }

  if (!GetWorld()) {
    return;
  }

  if (!TileBP) {
    UE_LOG(LogGrid, Error, TEXT("Tile Blueprint is not set for grid"));
    return;
  }

  TArray<AActor *> AttachedActors;
  GetAttachedActors(AttachedActors);
  if (!AttachedActors.IsEmpty()) {
    UE_LOG(LogGrid, Display, TEXT("Attached Actors is not empty: %i"),
           AttachedActors.Num());
    return;
  }

  for (size_t row = 0; row < Width; row++) {
    for (size_t col = 0; col < Length; col++) {
      size_t y = row * TileSize;
      size_t x = col * TileSize;

      FTransform SpawnTransform;
      SpawnTransform.SetLocation(FVector{x, y, 0});

      FActorSpawnParameters SpawnParams;
      SpawnParams.Owner = this;

      auto *SpawnedTile = GetWorld()->SpawnActor<AActor>(
          TileBP->GeneratedClass, SpawnTransform, SpawnParams);

      const FAttachmentTransformRules AttachmentRules(
          EAttachmentRule::KeepWorld, false);
      SpawnedTile->AttachToActor(this, AttachmentRules);
    }
  }
}
