#include "Level/Grid.h"

#include <algorithm>

#include "Editor.h"
#include "Engine/Blueprint.h"
#include "Engine/World.h"
#include "NavigationData.h"
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

} // namespace

AGrid::AGrid() {
  FEditorDelegates::OnNewActorsPlaced.AddUObject(this, &AGrid::OnEditorPlaced);
}

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

    FGridPosition GridPosition = GetTileGridPosition(Location.X, Location.Y);

    Tile->SetGridPosition(GridPosition);

    GridTilesIndex.Add(GridPosition, Tile);
  }
}

FGridPosition AGrid::GetTileGridPosition(const int x, const int y) const {
  const int row = y / TileSize;
  const int col = x / TileSize;

  return FGridPosition(row, col);
}

ATile *AGrid::GetTileByLocation(const int x, const int y) const {
  FGridPosition GridPosition = GetTileGridPosition(x, y);

  return GetTileByGridPos(GridPosition);
}

ATile *AGrid::GetTileByGridPos(const FGridPosition Pos) const {
  ATile *const *Value = GridTilesIndex.Find(Pos);

  if (Value == nullptr) {
    UE_LOG(LogGrid, Error, TEXT("Not found value by row %i, col %i"), Pos.row,
           Pos.col);
    return nullptr;
  }

  ATile *Tile = *Value;

  if (Tile == nullptr) {
    UE_LOG(LogGrid, Error, TEXT("Can't cast actor to tile at row %i, col %i"),
           Pos.row, Pos.col);
    return nullptr;
  }

  return Tile;
}

ATile *AGrid::GetScatterPoint(const EGhostType GhostType) const {
  for (const FGhostScatterPoint ScatterPoint : GhostScatterPoints) {
    if (ScatterPoint.GhostType == GhostType) {
      return ScatterPoint.Tile;
    }
  }

  return nullptr;
}

int AGrid::CalcTileDistance(const FGridPosition FirstCell,
                            const FGridPosition SecondCell) {

  if (FirstCell.row == SecondCell.row) {
    return std::abs(FirstCell.col - SecondCell.col);
  } else if (FirstCell.col == FirstCell.col) {
    return std::abs(FirstCell.row - SecondCell.row);
  } else {
    // Diagonal cells = max(Number of rows, Number of columns).
    uint rows = std::abs(FirstCell.row - FirstCell.row);
    uint cols = std::abs(SecondCell.col - SecondCell.col);

    return std::max(rows, cols);
  }
}

bool AGrid::IsCrossRoad(const ATile *Tile,
                        const FAdjacentTiles &AdjacentTiles) {
  int TilesIsNotWall = 0;

  // up
  if (AdjacentTiles.Up && (*AdjacentTiles.Up)->TileType == ETileType::Floor) {
    TilesIsNotWall += 1;
  }

  // right
  if (AdjacentTiles.Right &&
      (*AdjacentTiles.Right)->TileType == ETileType::Floor) {
    TilesIsNotWall += 1;
  }

  // left
  if (AdjacentTiles.Left &&
      (*AdjacentTiles.Left)->TileType == ETileType::Floor) {
    TilesIsNotWall += 1;
  }

  // down
  if (AdjacentTiles.Down &&
      (*AdjacentTiles.Down)->TileType == ETileType::Floor) {
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
FAdjacentTiles AGrid::GetAdjacentTiles(const ATile *Tile) {
  FGridPosition GridPosition = Tile->GetGridPosition();

  FGridPosition UpPosition =
      FGridPosition(GridPosition.row + 1, GridPosition.col);
  FGridPosition RightPosition =
      FGridPosition(GridPosition.row, GridPosition.col + 1);
  FGridPosition LeftPosition =
      FGridPosition(GridPosition.row, GridPosition.col - 1);
  FGridPosition DownPosition =
      FGridPosition(GridPosition.row - 1, GridPosition.col);

  return FAdjacentTiles{
      .Up = GridTilesIndex.Find(UpPosition),
      .Left = GridTilesIndex.Find(LeftPosition),
      .Right = GridTilesIndex.Find(RightPosition),
      .Down = GridTilesIndex.Find(DownPosition),
  };
}
