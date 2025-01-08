#include "Level/Grid.h"

#include "Editor.h"
#include "Engine/Blueprint.h"
#include "Engine/World.h"
#include "NavMesh/NavMeshBoundsVolume.h"
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
    // FIXME: not optimal
    if (Cast<ATile>(Actor) == nullptr) {
      continue;
    }

    FVector Location = Actor->GetActorLocation();

    const int row = Location.Y / TileSize;
    const int col = Location.X / TileSize;

    if (Actor == nullptr) {
      continue;
    }

    GridTilesIndex.Add(FGridPosition(row, col), Actor);
  }
}

FGridPosition AGrid::GetTileGridPosByLocation(const int x, const int y) const {
  const int row = y / TileSize;
  const int col = x / TileSize;

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
