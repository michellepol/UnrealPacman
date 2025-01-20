#include "AI/Tasks/BlueGhost/BlueChaseTask.h"

#include "AIController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Math/MathFwd.h"

#include "AI/GhostController.h"
#include "AI/Tasks/Common.h"
#include "Level/Tile.h"
#include "PacmanGameState.h"
#include "PacmanPlayer.h"

UAITask_BlueChase::UAITask_BlueChase() { NodeName = "Blue Ghost Chase Task"; }

EBTNodeResult::Type
UAITask_BlueChase::ExecuteTask(UBehaviorTreeComponent &OwnerComp,
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

  FGridPosition PacmanGridPosition =
      Grid->GetTileGridPosition(PacmanLocation.X, PacmanLocation.Y);

  ATile *Tile =
      GetPacmanFrontTile(PacmanGridPosition, *Pacman, *Grid, kTilesAheadPacman);
  if (!Tile) {
    UE_LOG(LogTask, Error, TEXT("Pacman Tile is Null"));
    return EBTNodeResult::Type::Failed;
  }

  FVector PacmanTileLocation = Tile->GetActorLocation();

  AGhost *RedGhost = PacmanGameState->GetGhost(EGhostType::kRed);
  if (!RedGhost) {
    UE_LOG(LogTask, Error, TEXT("RedGhost is Null"));
    return EBTNodeResult::Type::Failed;
  }

  FVector RedGhostLocation = RedGhost->GetActorLocation();

  FVector TargetVector(PacmanTileLocation.X - RedGhostLocation.X,
                       PacmanTileLocation.Y - RedGhostLocation.Y,
                       PacmanTileLocation.Z);

  // Multiply because target tile of ghost is end of TargetVector multiplied by
  // 2
  TargetVector.X *= 2;
  TargetVector.Y *= 2;

  FVector TargetLocation = RedGhostLocation + TargetVector;

  GhostController->MoveToTile(
      Grid->GetTileByLocation(TargetLocation.X, TargetLocation.Y));

  return EBTNodeResult::Type::Succeeded;
}

EBTNodeResult::Type
UAITask_BlueChase::AbortTask(UBehaviorTreeComponent &OwnerComp,
                             uint8 *NodeMemory) {
  Super::AbortTask(OwnerComp, NodeMemory);

  return EBTNodeResult::Type::Aborted;
}
