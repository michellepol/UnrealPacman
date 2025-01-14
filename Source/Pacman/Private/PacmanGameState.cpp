#include "PacmanGameState.h"

#include "Kismet/GameplayStatics.h"

#include "Engine/World.h"

DEFINE_LOG_CATEGORY(LogPacmanGameState);

APacmanGameState::APacmanGameState() {
  if (!GetWorld()) {
    UE_LOG(LogPacmanGameState, Error, TEXT("No world for game state"));
    return;
  }

  UWorld *World = GetWorld();

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
}

AGrid *APacmanGameState::GetGrid() { return Grid; }
