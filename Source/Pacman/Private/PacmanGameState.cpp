#include "PacmanGameState.h"

#include "Kismet/GameplayStatics.h"

#include "Engine/World.h"

DEFINE_LOG_CATEGORY(LogPacmanGameState);

APacmanGameState::APacmanGameState() = default;

void APacmanGameState::BeginPlay() {
  Super::BeginPlay();
  CacheLevelReferences();
}

void APacmanGameState::CacheLevelReferences() {
  UWorld *World = GetWorld();
  if (!World) {
    UE_LOG(LogPacmanGameState, Error, TEXT("No world for game state"));
    return;
  }

  TArray<AActor *> FoundGrids;
  UGameplayStatics::GetAllActorsOfClass(World, AGrid::StaticClass(),
                                        FoundGrids);

  UE_LOG(LogPacmanGameState, Display, TEXT("Found Grids: %i"),
         FoundGrids.Num());

  for (AActor *Actor : FoundGrids) {
    if (Actor) {
      Grid = Cast<AGrid>(Actor);
      break;
    }
  }

  TArray<AActor *> FoundGhosts;
  UGameplayStatics::GetAllActorsOfClass(World, AGhost::StaticClass(),
                                        FoundGhosts);

  for (AActor *PotentialGhost : FoundGhosts) {
    AGhost *Ghost = Cast<AGhost>(PotentialGhost);
    if (!Ghost) {
      continue;
    }
    Ghosts.Add(Ghost->GetGhostType(), Ghost);
  }
}

AGrid *APacmanGameState::GetGrid() { return Grid; }

AGhost *APacmanGameState::GetGhost(EGhostType GhostType) {
  AGhost **Value = Ghosts.Find(GhostType);

  if (!Value) {
    return nullptr;
  }

  return *Value;
}
