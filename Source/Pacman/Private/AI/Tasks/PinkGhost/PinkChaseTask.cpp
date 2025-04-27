#include "AI/Tasks/PinkGhost/PinkChaseTask.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Math/MathFwd.h"

#include "AI/GhostController.h"
#include "AI/Tasks/Common.h"
#include "Level/Tile.h"
#include "PacmanGameState.h"
#include "PacmanPlayer.h"

UAITask_PinkChase::UAITask_PinkChase() { NodeName = "Pink Ghost Chase Task"; }

EBTNodeResult::Type
UAITask_PinkChase::ExecuteTask(UBehaviorTreeComponent &OwnerComp,
                               uint8 *NodeMemory) {
  UWorld *World = OwnerComp.GetWorld();

  APacmanGameState *PacmanGameState = GetPacmanGameState(World);
  if (!PacmanGameState) {
    UE_LOG(LogTask, Error, TEXT("PacmanGameState is Null"));
    return EBTNodeResult::Type::Failed;
  }

  const AGrid *Grid = PacmanGameState->GetGrid();
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
  FGridPosition PacmanGridPosition = Grid->GetTileGridPosition(PacmanLocation);
  
  UE_LOG(LogTask, Log, TEXT("Pacman Grid Position: row %i, col %i"), PacmanGridPosition.row, PacmanGridPosition.col);

  ATile *Tile =
      GetPacmanFrontTile(PacmanGridPosition, *Pacman, *Grid, 4);

  if (Tile) {
    Tile->SetDebugMaterial();
  }

  AGhostController *GhostController = GetGhostController(OwnerComp);
  if (GhostController == nullptr) {
    UE_LOG(LogTask, Error, TEXT("AI Controller is Null"));
    return EBTNodeResult::Type::Failed;
  }

  const bool MoveResult = GhostController->MoveToTile(Tile);

  if (!MoveResult) {
    return EBTNodeResult::Type::Failed;
  }

  return EBTNodeResult::Type::Succeeded;
}

EBTNodeResult::Type
UAITask_PinkChase::AbortTask(UBehaviorTreeComponent &OwnerComp,
                             uint8 *NodeMemory) {
  Super::AbortTask(OwnerComp, NodeMemory);

  return EBTNodeResult::Type::Aborted;
}
