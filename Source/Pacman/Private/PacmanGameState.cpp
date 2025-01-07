#include "PacmanGameState.h"

#include "Kismet/GameplayStatics.h"

#include "Engine/World.h"

APacmanGameState::APacmanGameState() {
  if (!GetWorld()) {
    UE_LOG(LogTemp, Error, TEXT("No world for game state"));
    return;
  }

  UWorld *World = GetWorld();

  TArray<AActor *> FoundGrids;

  UGameplayStatics::GetAllActorsOfClass(World, AGrid::StaticClass(),
                                        FoundGrids);

  UE_LOG(LogTemp, Display, TEXT("Found Grids: %i"), FoundGrids.Num());

  for (AActor *Actor : FoundGrids) {
    if (Actor) {
      Grid = Cast<AGrid>(Actor);
      break;
    }
  }
}

AGrid *APacmanGameState::GetGrid() { return Grid; }
