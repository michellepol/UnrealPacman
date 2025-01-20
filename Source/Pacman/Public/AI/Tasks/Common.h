// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "AI/GhostController.h"
#include "Level/GridPosition.h"
#include "PacmanGameState.h"
#include "PacmanPlayer.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTask, Log, All)

AGhostController *GetGhostController(UBehaviorTreeComponent &OwnerComp);

APacmanPlayer *GetPacmanPlayer(UWorld *World);

APacmanGameState *GetPacmanGameState(UWorld *World);

///@brief Find location of tile which is 4 tiles ahead of Pacman current
/// direction
ATile *GetPacmanFrontTile(const FGridPosition &PacmanTilePosition,
                          const APacmanPlayer &Pacman, const AGrid &Grid,
                          const uint Offset);
