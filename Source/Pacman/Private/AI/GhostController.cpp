// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/GhostController.h"

#include "AIController.h"
#include "Engine/World.h"
#include "NavigationSystem.h"

#include "AI/Ghost.h"
#include "Level/Grid.h"
#include "Level/Tile.h"
#include "PacmanGameState.h"

DEFINE_LOG_CATEGORY(LogGhostController);

namespace {

//@brief Check if tile is behind Ghost
bool IsTileBehind(const AGhost &Ghost, const ATile &TargetTile,
                  const ATile &GhostTile) {

  FGridPosition TargetTilePos = TargetTile.GetGridPosition();
  FGridPosition GhostTilePos = GhostTile.GetGridPosition();

  switch (Ghost.GetDirection()) {
  case EDirection::kUp:
    return TargetTilePos.row < GhostTilePos.row;
    break;
  case EDirection::kDown:
    return TargetTilePos.row > GhostTilePos.row;
    break;
  case EDirection::kLeft:
    return TargetTilePos.col < GhostTilePos.col;
    break;
  case EDirection::kRight:
    return TargetTilePos.col > GhostTilePos.col;
  default:
    return false;
  }
}

} // namespace

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

  if (IsTileBehind(*Ghost, *Tile, *GhostTile)) {
    return false;
  }

  auto Result = MoveToLocation(Tile->GetActorLocation());

  return true;
}
