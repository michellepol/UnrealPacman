#include "Level/Grid.h"

#include "Editor.h"
#include "Engine/Blueprint.h"
#include "Engine/World.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "UObject/ObjectMacros.h"

#include "Level/Tile.h"

DEFINE_LOG_CATEGORY(LogGrid);

bool operator==(const FGridPosition lhs, const FGridPosition rhs) {
  return lhs.row == rhs.row && lhs.col == rhs.col;
}

AGrid::AGrid() {
  FEditorDelegates::OnNewActorsPlaced.AddUObject(this, &AGrid::OnEditorPlaced);
}

void AGrid::OnEditorPlaced(UObject *Object, const TArray<AActor *> &Actors) {
  bool GridPlaced = false;
  for (AActor *Actor : Actors) {
    if (Actor == this) {
      GridPlaced = true;
    }
  }

  if (GridPlaced == false) {
    return;
  }

  auto *World = GetWorld();

  if (!World) {
    return;
  }

  if (!TileBP) {
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

      auto *SpawnedTile = World->SpawnActor<AActor>(
          TileBP->GeneratedClass, SpawnTransform, SpawnParams);

      const FAttachmentTransformRules AttachmentRules(
          EAttachmentRule::KeepWorld, false);
      SpawnedTile->AttachToActor(this, AttachmentRules);
    }
  }
}

void DeleteRelatedActors(TArray<AActor *> Actors) {
  UE_LOG(LogGrid, Display, TEXT("Delete related actors: %i"), Actors.Num());

  for (AActor *Actor : Actors) {
    if (Actor) {
      Actor->Destroy();
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
    // FIXME: not optimal
    if (Cast<ATile>(Actor) == nullptr) {
      continue;
    }

    FVector Location = Actor->GetActorLocation();

    const int row = Location.X / TileSize;
    const int col = Location.Y / TileSize;

    if (Actor == nullptr) {
      continue;
    }

    GridTilesIndex.Add(FGridPosition(row, col), Actor);
  }
}

FGridPosition AGrid::GetTileGridPosByLocation(const int x, const int y) const {
  const int row = x / TileSize;
  const int col = y / TileSize;

  return FGridPosition(row, col);
}

ATile *AGrid::GetTileByLocation(const int x, const int y) const {
  FGridPosition GridPosition = GetTileGridPosByLocation(x, y);

  return GetTileByGridPos(GridPosition);
}

ATile *AGrid::GetTileByGridPos(const FGridPosition Pos) const {
  AActor *const *Value = GridTilesIndex.Find(Pos);

  if (Value == nullptr) {
    UE_LOG(LogGrid, Error, TEXT("Not found value by row %i, col %i"), Pos.row,
           Pos.col);
    return nullptr;
  }

  ATile *Tile = Cast<ATile>(*Value);

  if (Tile == nullptr) {
    UE_LOG(LogGrid, Error, TEXT("Can't cast actor to tile at row %i, col %i"),
           Pos.row, Pos.col);
    return nullptr;
  }

  return Tile;
}
