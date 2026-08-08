// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/GhostController.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Engine/World.h"
#include "NavigationData.h"
#include "NavigationSystem.h"

#include "AI/Ghost.h"
#include "Level/Grid.h"
#include "Level/PathFinding.h"
#include "Level/Tile.h"
#include "PacmanGameState.h"

DEFINE_LOG_CATEGORY(LogGhostController);

AGhostController::AGhostController() {}

void AGhostController::BeginPlay() {
  Super::BeginPlay();

  if (APacmanGameState *GameState =
          Cast<APacmanGameState>(GetWorld()->GetGameState())) {
    GameState->RegisterGhostController(this);
  }
}

AGrid *AGhostController::GetGrid() const {
  if (GetWorld() == nullptr) {
    return nullptr;
  }

  APacmanGameState *GameState =
      Cast<APacmanGameState>(GetWorld()->GetGameState());
  if (!GameState) {
    return nullptr;
  }

  return GameState->GetGrid();
}

bool AGhostController::MoveToTile(ATile *GoalTile) {
  FString ClassName = this->GetClass()->GetName();
  if (!GoalTile) {
    UE_LOG(LogGhostController, Warning,
           TEXT("Target Tile is NULL for Controller %s"),
           ClassName.GetCharArray().GetData());
    return false;
  }

  AGrid *Grid = GetGrid();

  if (!Grid) {
    UE_LOG(LogGhostController, Error, TEXT("No Grid class"));
    return false;
  }

  // If goal tile is wall, find adjacent floor tile
  ATile *Goal = GoalTile;
  if (Goal->GetTileType() == ETileType::Wall) {
    TArray<ATile *> AdjacentTiles = Grid->GetAdjacentTiles(GoalTile).ToArray();
    for (ATile *Tile : AdjacentTiles) {
      if (Tile && Tile->GetTileType() == ETileType::Floor) {
        Goal = Tile;
        break;
      }
    }
  }

  APawn *ControlledPawn = GetPawn();
  if (!ControlledPawn) {
    UE_LOG(LogGhostController, Error, TEXT("Controller has no pawn"));
    return false;
  }

  FVector GhostLocation = ControlledPawn->GetActorLocation();

  ATile *GhostTile = Grid->GetTileByLocation(GhostLocation);

  if (!GhostTile) {
    UE_LOG(LogGhostController, Error, TEXT("No Ghost Tile "));
    return false;
  }

  if (LastGhostLocation == GhostTile->GetGridPosition() &&
      LastGoalLocation == Goal->GetGridPosition()) {
    return false;
  }

  LastGhostLocation = GhostTile->GetGridPosition();
  LastGoalLocation = Goal->GetGridPosition();

  AGhost *Ghost = Cast<AGhost>(GetPawn());
  if (!Ghost) {
    UE_LOG(LogGhostController, Error,
           TEXT("Ghost Controller don't control Ghost"));
    return false;
  }

  auto Result = MoveToLocation(Goal->GetActorLocation(),
                               1,     // AcceptanceRadius
                               false, // bStopOnOverlap
                               true,  // bUsePathfinding
                               false, // bProjectDestinationToNavigation
                               true   // bCanStrafe
  );

  return true;
}

void AGhostController::FrigthenedMove() {
  AGrid *Grid = GetGrid();

  if (!Grid) {
    UE_LOG(LogGhostController, Error, TEXT("No Grid class"));
    return;
  }

  APawn *ControlledPawn = GetPawn();
  if (!ControlledPawn) {
    UE_LOG(LogGhostController, Error, TEXT("Controller has no pawn"));
    return;
  }

  FVector GhostLocation = ControlledPawn->GetActorLocation();

  ATile *GhostTile = Grid->GetTileByLocation(GhostLocation);

  if (!GhostTile) {
    UE_LOG(LogGhostController, Error, TEXT("No Ghost Tile "));
    return;
  }

  FGridPosition GridPosition = GhostTile->GetGridPosition();
  FAdjacentTiles AdjacentTiles = Grid->GetAdjacentTiles(GhostTile);

  const bool IsCrossRoadTile = Grid->IsCrossRoad(GhostTile, AdjacentTiles);

  if (IsCrossRoadTile) {
    int min = 0;
    int max = 3;
    //...
    int Direction = (rand() % (max - min)) + min;

    // RotateGhostToNewDirection();
  } else {
    FVector ForwardVector = ControlledPawn->GetActorForwardVector();
    MoveInDirection(ForwardVector);
  }
}

void AGhostController::FindPathForMoveRequest(
    const FAIMoveRequest &MoveRequest, FPathFindingQuery &Query,
    FNavPathSharedPtr &OutPath) const {
  UNavigationSystemV1 *NavSys =
      FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

  if (!NavSys) {
    UE_LOG(LogGhostController, Error,
           TEXT("Trying to find path to %s resulted in Error"),
           MoveRequest.IsMoveToActorRequest()
               ? *GetNameSafe(MoveRequest.GetGoalActor())
               : *MoveRequest.GetGoalLocation().ToString());

    return;
  }

  AGrid *Grid = GetGrid();

  if (!Grid) {
    UE_LOG(LogGhostController, Error, TEXT("No Grid class"));
    return;
  }

  APawn *ControlledPawn = GetPawn();
  if (!ControlledPawn) {
    UE_LOG(LogGhostController, Error, TEXT("Controller has no pawn"));
    return;
  }

  FVector GhostLocation = ControlledPawn->GetActorLocation();

  ATile *StartTile = Grid->GetTileByLocation(GhostLocation);

  if (!StartTile) {
    UE_LOG(LogGhostController, Error, TEXT("No Start Tile"));
    return;
  }

  FVector GoalLocation = MoveRequest.GetGoalLocation();

  ATile *GoalTile = Grid->GetTileByLocation(GoalLocation);

  if (!GoalTile) {
    UE_LOG(LogGhostController, Error, TEXT("No Goal Tile"));
    return;
  }

  const FGridPosition StartPosition = StartTile->GetGridPosition();
  FPathFindingResult PathResult;

  const bool bMustNotReverse =
      bHasLastPathDirection && !bAllowReverseOnNextPath;
  if (bMustNotReverse) {
    const FGridPosition ReverseTile = {
        StartPosition.row - LastPathDirection.row,
        StartPosition.col - LastPathDirection.col};
    TArray<FGridPosition> Path =
        AStar(Grid->GetIntGrid(), StartPosition, GoalTile->GetGridPosition(),
              &ReverseTile);

    if (Path.Num() > 1) {
      TArray<FVector3d> Points;
      for (const FGridPosition &Position : Path) {
        if (ATile *Tile = Grid->GetTileByGridPosition(Position)) {
          Points.Push(Tile->GetActorLocation());
        }
      }
      PathResult.Result = Points.Num() > 1 ? ENavigationQueryResult::Success
                                            : ENavigationQueryResult::Fail;
      PathResult.Path = MakeShared<FNavigationPath>(FNavigationPath(Points));
    }
  }

  if (!bMustNotReverse &&
      (!PathResult.IsSuccessful() || !PathResult.Path.IsValid())) {
    PathResult = FindPath(StartTile, GoalTile, Grid);
  }

  bAllowReverseOnNextPath = false;

  if (PathResult.IsSuccessful() && PathResult.Path.IsValid()) {
    OutPath = PathResult.Path;
    CurrentPath = PathResult.Path;

    const TArray<FNavPathPoint> &Points = CurrentPath->GetPathPoints();
    if (Points.Num() > 1) {
      if (ATile *NextTile = Grid->GetTileByLocation(Points[1].Location)) {
        const FGridPosition NextPosition = NextTile->GetGridPosition();
        LastPathDirection = {NextPosition.row - StartPosition.row,
                             NextPosition.col - StartPosition.col};
        bHasLastPathDirection = true;
      }
    }
    return;
  }

  UE_LOG(LogGhostController, Error,
         TEXT("Trying to find path to %s resulted in Error"),
         MoveRequest.IsMoveToActorRequest()
             ? *GetNameSafe(MoveRequest.GetGoalActor())
             : *MoveRequest.GetGoalLocation().ToString());
}

FNavPathSharedPtr AGhostController::GetCurrentPath() { return CurrentPath; }

bool AGhostController::SetGhostMode(EGhostMode NewMode) {
  static const FName GhostModeKey(TEXT("GhostMode"));
  UBlackboardComponent *BlackboardComponent = GetBlackboardComponent();
  if (!BlackboardComponent) {
    UE_LOG(LogGhostController, Warning,
           TEXT("Ghost controller %s has no Blackboard component"), *GetName());
    return false;
  }

  if (BlackboardComponent->GetKeyID(GhostModeKey) == FBlackboard::InvalidKey) {
    UE_LOG(LogGhostController, Warning,
           TEXT("Blackboard for %s must define enum key GhostMode"), *GetName());
    return false;
  }

  BlackboardComponent->SetValueAsEnum(GhostModeKey, static_cast<uint8>(NewMode));
  LastPublishedGhostMode = static_cast<uint8>(NewMode);
  return true;
}

void AGhostController::ReverseDirection() {
  StopMovement();
  bAllowReverseOnNextPath = true;
  bHasLastPathDirection = false;

  if (APawn *ControlledPawn = GetPawn()) {
    FRotator Rotation = ControlledPawn->GetActorRotation();
    Rotation.Yaw = FRotator::NormalizeAxis(Rotation.Yaw + 180.f);
    ControlledPawn->SetActorRotation(Rotation);
  }

  LastGoalLocation = FGridPosition();
  LastGhostLocation = FGridPosition();
}

void AGhostController::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  APacmanGameState *GameState =
      GetWorld() ? Cast<APacmanGameState>(GetWorld()->GetGameState()) : nullptr;
  if (!GameState) {
    return;
  }

  const uint8 CurrentMode = static_cast<uint8>(GameState->GetGhostMode());
  if (LastPublishedGhostMode != CurrentMode) {
    SetGhostMode(GameState->GetGhostMode());
  }
}

void AGhostController::MoveInDirection(FVector Direction, float Scale) {
  if (APawn *ControlledPawn = GetPawn()) {
    ControlledPawn->AddMovementInput(Direction, Scale);
  }
}
