// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/GhostController.h"

#include "AIController.h"
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
    TArray<ATile * > AdjacentTiles = Grid->GetAdjacentTiles(GoalTile).ToArray();
    for (ATile * Tile : AdjacentTiles) {
      if (Tile && Tile->GetTileType() == ETileType::Floor) {
        Goal = Tile;
        break;
      }
    }
  }

  FVector GhostLocation = GetPawn()->GetActorLocation();

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

  FVector GhostLocation = GetPawn()->GetActorLocation();

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
    FVector ForwardVector = GetPawn()->GetActorForwardVector();
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

  FVector GhostLocation = GetPawn()->GetActorLocation();

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

  FPathFindingResult PathResult = FindPath(StartTile, GoalTile, Grid);

  if (PathResult.IsSuccessful() && PathResult.Path.IsValid()) {
    OutPath = PathResult.Path;
    return;
  }

  UE_LOG(LogGhostController, Error,
         TEXT("Trying to find path to %s resulted in Error"),
         MoveRequest.IsMoveToActorRequest()
             ? *GetNameSafe(MoveRequest.GetGoalActor())
             : *MoveRequest.GetGoalLocation().ToString());
}

void AGhostController::Tick(float DeltaTime) {}

void AGhostController::MoveInDirection(FVector Direction, float Scale) {
  GetPawn()->AddMovementInput(Direction, Scale);
}
