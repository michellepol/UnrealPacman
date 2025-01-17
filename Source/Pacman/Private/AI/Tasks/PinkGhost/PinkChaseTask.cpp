#include "AI/Tasks/PinkGhost/PinkChaseTask.h"

#include "AIController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Math/MathFwd.h"

#include "AI/GhostController.h"
#include "AI/Tasks/Common.h"
#include "Level/Tile.h"
#include "PacmanGameState.h"
#include "PacmanPlayer.h"

namespace {

///@brief Find location of tile which is 4 tiles ahead of Pacman current
/// direction
ATile *FindTargetTile(const FGridPosition &PacmanTilePosition,
                      const APacmanPlayer &Pacman, const AGrid &Grid) {
  constexpr static int kTilesAheadPacman = 4;

  FGridPosition TargetTilePosition = PacmanTilePosition;

  switch (Pacman.GetDirection()) {
  case EDirection::kUp:
    TargetTilePosition.row += kTilesAheadPacman;
    break;
  case EDirection::kDown:
    TargetTilePosition.row -= kTilesAheadPacman;
    break;
  case EDirection::kLeft:
    TargetTilePosition.col += kTilesAheadPacman;
    break;
  case EDirection::kRight:
    TargetTilePosition.col -= kTilesAheadPacman;
    break;
  default:
    // do nothing
    break;
  }

  return Grid.GetTile(TargetTilePosition);
}

} // namespace

UAITask_PinkChase::UAITask_PinkChase() { NodeName = "Pink Ghost Chase Task"; }

EBTNodeResult::Type
UAITask_PinkChase::ExecuteTask(UBehaviorTreeComponent &OwnerComp,
                               uint8 *NodeMemory) {
  UWorld *World = OwnerComp.GetWorld();

  const AGrid *Grid = GetGrid(World);
  if (!Grid) {
    UE_LOG(LogTask, Error, TEXT("Grid is Null"));
    return EBTNodeResult::Type::Failed;
  }

  APacmanPlayer *Pacman = GetPacmanPlayer(World);
  if (Pacman == nullptr) {
    UE_LOG(LogTask, Error, TEXT("Player pawn is Null"));
    return EBTNodeResult::Type::Failed;
  }

  FVector PacmanLocation = Pacman->GetActorLocation();
  FGridPosition PacmanGridPosition =
      Grid->GetTileGridPosition(PacmanLocation.X, PacmanLocation.Y);

  ATile *Tile = FindTargetTile(PacmanGridPosition, *Pacman, *Grid);

  if (Tile) {
    Tile->SetDebugMaterial();
  }

  AGhostController *GhostController = GetGhostController(OwnerComp);
  if (GhostController == nullptr) {
    UE_LOG(LogTask, Error, TEXT("AI Controller is Null"));
    return EBTNodeResult::Type::Failed;
  }

  GhostController->MoveToTile(Tile);

  return EBTNodeResult::Type::Succeeded;
}

EBTNodeResult::Type
UAITask_PinkChase::AbortTask(UBehaviorTreeComponent &OwnerComp,
                             uint8 *NodeMemory) {
  Super::AbortTask(OwnerComp, NodeMemory);

  return EBTNodeResult::Type::Aborted;
}
