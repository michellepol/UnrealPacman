// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "CoreMinimal.h"
#include "Level/GridPosition.h"

#include "GhostController.generated.h"

class ATile;
class AGrid;

DECLARE_LOG_CATEGORY_EXTERN(LogGhostController, Log, All);

UCLASS()
class PACMAN_API AGhostController : public AAIController {
  GENERATED_BODY()

public:
  AGhostController();

  virtual void BeginPlay() override;

  bool MoveToTile(ATile *Tile);

  void FrigthenedMove();

  void Tick(float DeltaTime) override;

  FGridPosition LastGoalLocation;
  FGridPosition LastGhostLocation;

  void FindPathForMoveRequest(const FAIMoveRequest &MoveRequest,
                              FPathFindingQuery &Query,
                              FNavPathSharedPtr &OutPath) const override;

  FNavPathSharedPtr GetCurrentPath();

  /** Writes the shared game-state mode to this controller's Blackboard. */
  bool SetGhostMode(EGhostMode NewMode);

  /** Stops the current move and makes the ghost face its opposite direction. */
  void ReverseDirection();

private:
  AGrid *GetGrid() const;

  void MoveInDirection(FVector Direction, float Speed = 0.1f);

  /** Direction of the first step selected by the last grid path. */
  FGridPosition LastPathDirection;
  bool bHasLastPathDirection = false;

  /** Set solely by a Scatter/Chase transition. */
  bool bAllowReverseOnNextPath = false;

  mutable FNavPathSharedPtr CurrentPath;
  uint8 LastPublishedGhostMode = MAX_uint8;
};
