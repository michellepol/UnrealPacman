#include "Level/Grid.h"

#include "Editor.h"
#include "Engine/Blueprint.h"
#include "Engine/World.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "UObject/ObjectMacros.h"

DEFINE_LOG_CATEGORY(LogGrid);

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

  UE_LOG(LogGrid, Display, TEXT("IsEditorPlaced called"));

  TArray<AActor *> AttachedActors;
  GetAttachedActors(AttachedActors);
  if (!AttachedActors.IsEmpty()) {
    UE_LOG(LogGrid, Display, TEXT("Attached Actors is not empty: %i"),
           AttachedActors.Num());
    return;
  }

  for (size_t i = 0; i < Width; i++) {
    for (size_t j = 0; j < Length; j++) {
      size_t y = i * TileSize;
      size_t x = j * TileSize;

      FTransform SpawnTransform;
      SpawnTransform.SetLocation(FVector{x, y, 0});

      FActorSpawnParameters SpawnParams;
      SpawnParams.Owner = this;

      auto *SpawnedTile = World->SpawnActor<AActor>(
          TileBP->GeneratedClass, SpawnTransform, SpawnParams);

      const FAttachmentTransformRules AttachmentRules(
          EAttachmentRule::KeepWorld, false);
      SpawnedTile->AttachToActor(this, AttachmentRules);

      Tiles.Add(SpawnedTile);
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
  DeleteRelatedActors(Tiles);
}
