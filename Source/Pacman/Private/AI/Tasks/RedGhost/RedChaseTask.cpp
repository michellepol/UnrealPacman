#include "AI/Tasks/RedGhost/RedChaseTask.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Math/MathFwd.h"

#include "AI/GhostController.h"
#include "AI/Tasks/Common.h"
#include "PacmanGameState.h"
#include "Level/Tile.h"
#include "PacmanPlayer.h"

UAITask_RedChase::UAITask_RedChase() { NodeName = "Red Ghost Chase Task"; }

EBTNodeResult::Type
UAITask_RedChase::ExecuteTask(UBehaviorTreeComponent &OwnerComp,
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

  AGhostController *GhostController = GetGhostController(OwnerComp);
  if (GhostController == nullptr) {
    UE_LOG(LogTask, Error, TEXT("AI Controller is Null"));
    return EBTNodeResult::Type::Failed;
  }

  FVector PacmanLocation = Pacman->GetActorLocation();

  ATile* TargetTile = Grid->GetTileByLocation(PacmanLocation);

  const bool MoveResult =
      GhostController->MoveToTile(TargetTile);

  if (!MoveResult) {
    return EBTNodeResult::Type::Failed;
  }

  return EBTNodeResult::Type::Succeeded;
}

EBTNodeResult::Type
UAITask_RedChase::AbortTask(UBehaviorTreeComponent &OwnerComp,
                            uint8 *NodeMemory) {
  Super::AbortTask(OwnerComp, NodeMemory);

  return EBTNodeResult::Type::Aborted;
}
