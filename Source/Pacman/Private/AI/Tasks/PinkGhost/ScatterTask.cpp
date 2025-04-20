#include "AI/Tasks/PinkGhost/ScatterTask.h"

#include "AIController.h"
#include "Engine/World.h"

#include "AI/GhostController.h"
#include "Level/Tile.h"
#include "PacmanGameState.h"
#include "Utils/Math.h"

DEFINE_LOG_CATEGORY(LogPinkScatterTask)

UAITask_PinkScatter::UAITask_PinkScatter() {
  static const char *kNodeName = "Pink Ghost Scatter Task";
  NodeName = kNodeName;
}

EBTNodeResult::Type
UAITask_PinkScatter::ExecuteTask(UBehaviorTreeComponent &OwnerComp,
                                 uint8 *NodeMemory) {
  // Reach target tile
  // If reach - go to in way to reach target again
  UWorld *World = OwnerComp.GetWorld();
  if (!World) {
    UE_LOG(LogPinkScatterTask, Error, TEXT("World is Null"));
    return EBTNodeResult::Type::Failed;
  }

  AGameStateBase *GameState = World->GetGameState();
  if (!GameState) {
    UE_LOG(LogPinkScatterTask, Error, TEXT("GameState is Null"));
    return EBTNodeResult::Type::Failed;
  }

  APacmanGameState *PacmanGameState = Cast<APacmanGameState>(GameState);
  if (!PacmanGameState) {
    UE_LOG(LogPinkScatterTask, Error,
           TEXT("Can't cast BaseGameState to Pacman State"));
    return EBTNodeResult::Type::Failed;
  }

  const AGrid *Grid = PacmanGameState->GetGrid();
  if (!Grid) {
    UE_LOG(LogPinkScatterTask, Error, TEXT("Grid is Null"));
    return EBTNodeResult::Type::Failed;
  }

  /*
  ATile *Tile = Grid->GetScatterPoint(EGhostType::kPink);

  if (!Tile) {
    UE_LOG(LogPinkScatterTask, Error, TEXT("Scatter point is Null"));
    return EBTNodeResult::Type::Failed;
  }

  AGhostController *GhostController =
      Cast<AGhostController>(OwnerComp.GetAIOwner());
  if (GhostController == nullptr) {
    UE_LOG(LogPinkScatterTask, Error, TEXT("AI Controller is Null"));
    return EBTNodeResult::Type::Failed;
  }

  GhostController->MoveToTile(Tile);
  */

  return EBTNodeResult::Type::InProgress;
}

EBTNodeResult::Type
UAITask_PinkScatter::AbortTask(UBehaviorTreeComponent &OwnerComp,
                               uint8 *NodeMemory) {
  Super::AbortTask(OwnerComp, NodeMemory);

  return EBTNodeResult::Type::Aborted;
}
