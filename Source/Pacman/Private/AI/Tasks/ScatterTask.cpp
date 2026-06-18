#include "AI/Tasks/ScatterTask.h"

#include "Engine/World.h"

#include "AI/Ghost.h"
#include "AI/GhostController.h"
#include "AI/Tasks/Common.h"
#include "Level/Grid.h"
#include "Level/Tile.h"
#include "PacmanGameState.h"

UAITask_Scatter::UAITask_Scatter() { NodeName = "Scatter Task"; }

EBTNodeResult::Type
UAITask_Scatter::ExecuteTask(UBehaviorTreeComponent &OwnerComp,
                              uint8 *NodeMemory) {
  UWorld *World = OwnerComp.GetWorld();

  APacmanGameState *PacmanGameState = GetPacmanGameState(World);
  if (!PacmanGameState) {
    UE_LOG(LogTask, Error, TEXT("PacmanGameState is Null"));
    return EBTNodeResult::Failed;
  }

  const AGrid *Grid = PacmanGameState->GetGrid();
  if (!Grid) {
    UE_LOG(LogTask, Error, TEXT("Grid is Null"));
    return EBTNodeResult::Failed;
  }

  AGhostController *GhostController = GetGhostController(OwnerComp);
  if (!GhostController) {
    UE_LOG(LogTask, Error, TEXT("AI Controller is Null"));
    return EBTNodeResult::Failed;
  }

  AGhost *Ghost = Cast<AGhost>(GhostController->GetPawn());
  if (!Ghost) {
    UE_LOG(LogTask, Error, TEXT("Ghost is Null"));
    return EBTNodeResult::Failed;
  }

  ATile *TargetTile = Grid->GetScatterPoint(Ghost->GetGhostType());
  if (!TargetTile) {
    UE_LOG(LogTask, Error, TEXT("Scatter point is Null"));
    return EBTNodeResult::Failed;
  }

  const bool bMoveResult = GhostController->MoveToTile(TargetTile);

  if (!bMoveResult) {
    return EBTNodeResult::Failed;
  }

  return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type
UAITask_Scatter::AbortTask(UBehaviorTreeComponent &OwnerComp,
                             uint8 *NodeMemory) {
  Super::AbortTask(OwnerComp, NodeMemory);

  return EBTNodeResult::Aborted;
}