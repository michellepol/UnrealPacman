// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "Level/Grid.h"

#include "PacmanGameState.generated.h"

UCLASS()
class PACMAN_API APacmanGameState : public AGameStateBase {
  GENERATED_BODY()

public:
  APacmanGameState();

  AGrid *GetGrid();

  UPROPERTY(EditAnywhere, BlueprintReadWrite);
  AGrid *Grid = nullptr;
};
