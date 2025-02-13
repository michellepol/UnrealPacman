// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "CoreMinimal.h"
#include "GhostController.generated.h"

class ATile;
class AGrid;

DECLARE_LOG_CATEGORY_EXTERN(LogGhostController, Log, All);

UCLASS()
class PACMAN_API AGhostController : public AAIController {
  GENERATED_BODY()

public:
  AGhostController() = default;

  bool MoveToTile(ATile *Tile);

  void FrigthenedMove();

  void Tick(float DeltaTime) override;

  FVector CurrentDirection;

private:
  AGrid *GetGrid();
};
