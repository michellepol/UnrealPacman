// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/GhostController.h"

#include "AIController.h"
#include "Engine/World.h"
#include "NavigationData.h"
#include "NavigationSystem.h"

#include "AI/Ghost.h"
#include "Level/Grid.h"
#include "Level/Tile.h"
#include "PacmanGameState.h"

DEFINE_LOG_CATEGORY(LogGhostController);

AGrid *AGhostController::GetGrid() {
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

bool AGhostController::MoveToTile(ATile *Tile) {
  if (!Tile) {
    UE_LOG(LogGhostController, Error, TEXT("Target Tile is NULL"));
    return false;
  }

  AGrid *Grid = GetGrid();

  if (!Grid) {
    UE_LOG(LogGhostController, Error, TEXT("No Grid class"));
    return false;
  }

  FVector GhostLocation = GetPawn()->GetActorLocation();

  ATile *GhostTile = Grid->GetTileByLocation(GhostLocation.X, GhostLocation.Y);

  if (!GhostTile) {
    UE_LOG(LogGhostController, Error, TEXT("No Ghost Tile "));
    return false;
  }

  AGhost *Ghost = Cast<AGhost>(GetPawn());
  if (!Ghost) {
    UE_LOG(LogGhostController, Error,
           TEXT("Ghost Controller don't control Ghost"));
    return false;
  }

  auto Result = MoveToLocation(Tile->GetActorLocation(),
                               -1,    // AcceptanceRadius
                               true,  // bStopOnOverlap
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

  ATile *GhostTile = Grid->GetTileByLocation(GhostLocation.X, GhostLocation.Y);

  if (!GhostTile) {
    UE_LOG(LogGhostController, Error, TEXT("No Ghost Tile "));
    return;
  }

  const bool IsCrossRoadTile = Grid->IsCrossRoad(GhostTile);

  if (IsCrossRoadTile) {
    RotateGhostToNewDirection();
  } else {
    MoveForward();
  }
}

void AGhostController::Tick(float DeltaTime) {
  FVector ForwardVector = GetPawn()->GetActorForwardVector();
}

void AGhostController::MoveInDirection(FVector Direction, float Speed,
                                       float DeltaTile) {
  FVector Movement = Direction * Speed * DeltaTime;

  AddActorWorldOffs
}
