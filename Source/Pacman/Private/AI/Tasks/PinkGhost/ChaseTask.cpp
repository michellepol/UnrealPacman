#include "AI/Tasks/PinkGhost/ChaseTask.h"

#include "AIController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Math/MathFwd.h"

#include "Level/Tile.h"
#include "PacmanGameState.h"

DEFINE_LOG_CATEGORY(LogTask)

UPinkGhostChase_Task::UPinkGhostChase_Task() {
  NodeName = "Pink Ghost Chase Task"; // Name of this task for easy debugging
}

FVector CalculateTargetLocation(const FGridPosition PacmanTilePosition,
                                const FVector &PacmanVelocity,
                                const AGrid &Grid) {
  constexpr static int kTilesAheadPacman = 4;

  FGridPosition TargetTilePosition = PacmanTilePosition;

  FVector UpDirection = FVector{0.0f, 1.0f, 0.0f};
  FVector DownDirection = FVector{0.0f, -1.0f, 0.0f};
  FVector RightDirection = FVector{-1.0f, 0.0f, 0.0f};

  if (FVector::DotProduct(PacmanVelocity, UpDirection) > 0.0f) {
    TargetTilePosition.row += kTilesAheadPacman;
  } else if (FVector::DotProduct(PacmanVelocity, DownDirection) > 0.0f) {
    TargetTilePosition.row -= kTilesAheadPacman;
  } else if (FVector::DotProduct(PacmanVelocity, RightDirection) > 0.0f) {
    TargetTilePosition.col -= kTilesAheadPacman;
  } else {
    TargetTilePosition.col += kTilesAheadPacman;
  }

  ATile *Tile = Grid.GetTileByGridPos(TargetTilePosition);

  if (!Tile) {
    UE_LOG(LogTask, Error, TEXT("Not valid Tile"));
    return FVector();
  }

  Tile->SetDebugMaterial();

  return Tile->GetActorLocation();
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

  // Get the player pawn from the controller
  APawn *PlayerPawn = PlayerController->GetPawn();
  if (PlayerPawn == nullptr) {
    UE_LOG(LogTask, Error, TEXT("Player pawn is Null"));
    return EBTNodeResult::Failed;
  }

  FVector PacmanLocation = PlayerPawn->GetActorLocation();

  FGridPosition PacmanGridPosition =
      Grid->GetTileGridPosByLocation(PacmanLocation.X, PacmanLocation.Y);

  FVector Velocity = PlayerPawn->GetVelocity();

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
