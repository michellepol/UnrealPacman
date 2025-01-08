#include "AI/Tasks/PinkGhost/ChaseTask.h"

#include "AIController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Math/MathFwd.h"

#include "Level/Tile.h"
#include "PacmanGameState.h"
#include "Utils/Math.h"

DEFINE_LOG_CATEGORY(LogTask)

namespace {

///@brief Find location of tile which is 4 tiles ahead of Pacman current
/// direction
FVector CalculateTargetLocation(const FGridPosition PacmanTilePosition,
                                FVector PacmanVelocity, const AGrid &Grid) {
  constexpr static int kTilesAheadPacman = 4;
  const static FVector UpDirection = FVector{0.0f, 1.0f, 0.0f};
  const static FVector DownDirection = FVector{0.0f, -1.0f, 0.0f};
  const static FVector RightDirection = FVector{-1.0f, 0.0f, 0.0f};
  const static FVector LeftDirection = FVector{1.0f, 0.0f, 0.0f};

  FGridPosition TargetTilePosition = PacmanTilePosition;

  PacmanVelocity.Normalize();

  if (IsSameDirection(UpDirection, PacmanVelocity)) {
    TargetTilePosition.row += kTilesAheadPacman;
  } else if (IsSameDirection(DownDirection, PacmanVelocity)) {
    TargetTilePosition.row -= kTilesAheadPacman;
  } else if (IsSameDirection(RightDirection, PacmanVelocity)) {
    TargetTilePosition.col -= kTilesAheadPacman;
  } else if (IsSameDirection(LeftDirection, PacmanVelocity)) {
    TargetTilePosition.col += kTilesAheadPacman;
  } else {
  }

  ATile *Tile = Grid.GetTileByGridPos(TargetTilePosition);

  if (!Tile) {
    UE_LOG(LogTask, Error,
           TEXT("Not valid Tile for Pink Ghost Target Location"));
    return FVector();
  }

  Tile->SetDebugMaterial();

  return Tile->GetActorLocation();
}

} // namespace

UPinkGhostChase_Task::UPinkGhostChase_Task() {
  NodeName = "Pink Ghost Chase Task";
}

EBTNodeResult::Type
UPinkGhostChase_Task::ExecuteTask(UBehaviorTreeComponent &OwnerComp,
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

  APlayerController *PlayerController =
      UGameplayStatics::GetPlayerController(World, 0);
  if (PlayerController == nullptr) {
    UE_LOG(LogTask, Error, TEXT("Player controller is Null"));
    return EBTNodeResult::Failed;
  }

  APawn *Pacman = PlayerController->GetPawn();
  if (Pacman == nullptr) {
    UE_LOG(LogTask, Error, TEXT("Player pawn is Null"));
    return EBTNodeResult::Failed;
  }

  FVector PacmanLocation = Pacman->GetActorLocation();

  FGridPosition PacmanGridPosition =
      Grid->GetTileGridPosByLocation(PacmanLocation.X, PacmanLocation.Y);

  FVector Velocity = Pacman->GetVelocity();

  FVector TargetLocation =
      CalculateTargetLocation(PacmanGridPosition, Velocity, *Grid);

  AAIController *AIController = OwnerComp.GetAIOwner();
  if (AIController == nullptr) {
    UE_LOG(LogTask, Error, TEXT("AI Controller is Null"));
    return EBTNodeResult::Type::Failed;
  }

  AIController->MoveToLocation(TargetLocation);

  return EBTNodeResult::Type::Succeeded;
}

EBTNodeResult::Type
UPinkGhostChase_Task::AbortTask(UBehaviorTreeComponent &OwnerComp,
                                uint8 *NodeMemory) {

  // Call the base class's Abort function if you want default behavior
  Super::AbortTask(OwnerComp, NodeMemory);

  return EBTNodeResult::Type::Aborted;
}
