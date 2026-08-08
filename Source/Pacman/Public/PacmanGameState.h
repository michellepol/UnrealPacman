// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "Level/Grid.h"
#include "AI/Ghost.h"

#include "PacmanGameState.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPacmanGameState, Log, All);

class AGhostController;

struct FGhostWave {
  EGhostMode Mode;
  float Duration;
};

UCLASS()
class PACMAN_API APacmanGameState : public AGameStateBase {
  GENERATED_BODY()

public:
  APacmanGameState();

protected:
  virtual void BeginPlay() override;

  void CacheLevelReferences();
  void StartGhostWaveSchedule();
  void AdvanceGhostWave();
  void EndFrightenedMode();
  void SetGhostMode(EGhostMode NewMode);
  void PublishGhostMode(AGhostController *Controller) const;

public:
  AGrid *GetGrid();

  AGhost *GetGhost(EGhostType GhostType);

  void RegisterGhostController(AGhostController *GhostController);

  /** Temporarily pauses the shared Scatter/Chase schedule. */
  UFUNCTION(BlueprintCallable, Category = "Ghosts")
  void StartFrightenedMode(float Duration);

  UFUNCTION(BlueprintPure, Category = "Ghosts")
  EGhostMode GetGhostMode() const { return GhostMode; }

public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite);
  AGrid *Grid = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite);
  TMap<EGhostType, AGhost *> Ghosts;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ghosts")
  EGhostMode GhostMode = EGhostMode::Scatter;

private:
  int32 GhostWaveIndex = 0;
  bool bFrightenedModeActive = false;
  FTimerHandle GhostWaveTimerHandle;
  FTimerHandle FrightenedTimerHandle;
};
