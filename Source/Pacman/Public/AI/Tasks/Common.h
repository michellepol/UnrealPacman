// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "AI/GhostController.h"
#include "PacmanPlayer.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTask, Log, All)

AGhostController *GetGhostController(UBehaviorTreeComponent &OwnerComp);

APacmanPlayer *GetPacmanPlayer(UWorld *World);

AGrid *GetGrid(UWorld *World);
