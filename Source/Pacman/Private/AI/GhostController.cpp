// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/GhostController.h"

#include "AIController.h"
#include "Engine/World.h"
#include "NavigationSystem.h"

#include "Level/Grid.h"
#include "Level/Tile.h"
#include "PacmanGameState.h"

DEFINE_LOG_CATEGORY(LogGhostController);

namespace {

//@brief Check if tile is behind Ghost
bool IsTileBehing(const ATile TargetTile, const ATile &GhostTile) {}

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
    return false;
  }

  AGrid *Grid = GetGrid();

  if (!Grid) {
    UE_LOG(LogGhostController, Error, TEXT("No Grid class"));
  }

  if (true) { // if tile is backward -> no move
    return false;
  }

  auto Result = MoveToLocation(Tile->GetActorLocation());

  return true;
}
