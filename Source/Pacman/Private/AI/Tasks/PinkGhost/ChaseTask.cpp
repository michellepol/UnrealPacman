#include "AI/Tasks/PinkGhost/ChaseTask.h"

#include "AIController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Math/MathFwd.h"

#include "AI/GhostController.h"
#include "Level/Tile.h"
#include "PacmanGameState.h"
#include "PacmanPlayer.h"

DEFINE_LOG_CATEGORY(LogTask)

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

AGhostController *GetGhostController(UBehaviorTreeComponent &OwnerComp) {
  AAIController *AIController = OwnerComp.GetAIOwner();
  if (AIController == nullptr) {
    UE_LOG(LogTask, Error, TEXT("AI Controller is Null"));
    return nullptr;
  }

  return Cast<AGhostController>(AIController);
}

APacmanPlayer *GetPacmanPlayer(UWorld *World) {
  APlayerController *PlayerController =
      UGameplayStatics::GetPlayerController(World, 0);
  if (PlayerController == nullptr) {
    UE_LOG(LogTask, Error, TEXT("Player controller is Null"));
    return nullptr;
  }

  return Cast<APacmanPlayer>(PlayerController->GetPawn());
}

} // namespace

UAITask_PinkChase::UAITask_PinkChase() { NodeName = "Pink Ghost Chase Task"; }

EBTNodeResult::Type
UAITask_PinkChase::ExecuteTask(UBehaviorTreeComponent &OwnerComp,
                               uint8 *NodeMemory) {
  UWorld *World = OwnerComp.GetWorld();
  if (!World) {
    UE_LOG(LogTask, Error, TEXT("World is Null"));
    return EBTNodeResult::Type::Failed;
  }

  AGameStateBase *GameState = World->GetGameState();
  if (!GameState) {
    UE_LOG(LogTask, Error, TEXT("GameState is Null"));
    return EBTNodeResult::Type::Failed;
  }

  APacmanGameState *PacmanGameState = Cast<APacmanGameState>(GameState);
  if (!PacmanGameState) {
    UE_LOG(LogTask, Error, TEXT("Can't cast BaseGameState to Pacman State"));
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
