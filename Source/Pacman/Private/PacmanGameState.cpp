#include "PacmanGameState.h"

#include "AI/GhostController.h"
#include "Kismet/GameplayStatics.h"

#include "Engine/World.h"

DEFINE_LOG_CATEGORY(LogPacmanGameState);

namespace {
const TArray<FGhostWave> GhostWaves = {
    {EGhostMode::Scatter, 7.f}, {EGhostMode::Chase, 20.f},
    {EGhostMode::Scatter, 7.f}, {EGhostMode::Chase, 20.f},
    {EGhostMode::Scatter, 5.f}, {EGhostMode::Chase, 20.f},
    {EGhostMode::Scatter, 5.f},
    {EGhostMode::Chase, TNumericLimits<float>::Max()}};
}

APacmanGameState::APacmanGameState() = default;

void APacmanGameState::BeginPlay() {
  Super::BeginPlay();
  CacheLevelReferences();
  StartGhostWaveSchedule();
}

void APacmanGameState::StartGhostWaveSchedule() {
  GhostWaveIndex = 0;
  bFrightenedModeActive = false;
  SetGhostMode(GhostWaves[GhostWaveIndex].Mode);
  GetWorldTimerManager().SetTimer(GhostWaveTimerHandle, this,
                                  &APacmanGameState::AdvanceGhostWave,
                                  GhostWaves[GhostWaveIndex].Duration, false);
}

void APacmanGameState::AdvanceGhostWave() {
  GhostWaveIndex = FMath::Min(GhostWaveIndex + 1, GhostWaves.Num() - 1);
  const FGhostWave &NextWave = GhostWaves[GhostWaveIndex];
  SetGhostMode(NextWave.Mode);

  if (GhostWaveIndex < GhostWaves.Num() - 1) {
    GetWorldTimerManager().SetTimer(GhostWaveTimerHandle, this,
                                    &APacmanGameState::AdvanceGhostWave,
                                    NextWave.Duration, false);
  }
}

void APacmanGameState::StartFrightenedMode(float Duration) {
  if (Duration <= 0.f) {
    return;
  }

  bFrightenedModeActive = true;
  GetWorldTimerManager().PauseTimer(GhostWaveTimerHandle);
  SetGhostMode(EGhostMode::Frightened);
  GetWorldTimerManager().SetTimer(FrightenedTimerHandle, this,
                                  &APacmanGameState::EndFrightenedMode,
                                  Duration, false);
}

void APacmanGameState::EndFrightenedMode() {
  if (!bFrightenedModeActive) {
    return;
  }

  bFrightenedModeActive = false;
  SetGhostMode(GhostWaves[GhostWaveIndex].Mode);
  GetWorldTimerManager().UnPauseTimer(GhostWaveTimerHandle);
}

void APacmanGameState::SetGhostMode(EGhostMode NewMode) {
  const EGhostMode PreviousMode = GhostMode;
  GhostMode = NewMode;

  TArray<AActor *> Controllers;
  UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGhostController::StaticClass(),
                                        Controllers);
  for (AActor *Actor : Controllers) {
    AGhostController *Controller = Cast<AGhostController>(Actor);
    if (!Controller) {
      continue;
    }

    PublishGhostMode(Controller);
    if ((PreviousMode == EGhostMode::Scatter && NewMode == EGhostMode::Chase) ||
        (PreviousMode == EGhostMode::Chase && NewMode == EGhostMode::Scatter)) {
      Controller->ReverseDirection();
    }
  }
}

void APacmanGameState::PublishGhostMode(AGhostController *Controller) const {
  if (Controller) {
    Controller->SetGhostMode(GhostMode);
  }
}

void APacmanGameState::CacheLevelReferences() {
  UWorld *World = GetWorld();
  if (!World) {
    UE_LOG(LogPacmanGameState, Error, TEXT("No world for game state"));
    return;
  }

  TArray<AActor *> FoundGrids;
  UGameplayStatics::GetAllActorsOfClass(World, AGrid::StaticClass(),
                                        FoundGrids);

  UE_LOG(LogPacmanGameState, Display, TEXT("Found Grids: %i"),
         FoundGrids.Num());

  for (AActor *Actor : FoundGrids) {
    if (Actor) {
      Grid = Cast<AGrid>(Actor);
      break;
    }
  }

  TArray<AActor *> FoundGhosts;
  UGameplayStatics::GetAllActorsOfClass(World, AGhost::StaticClass(),
                                        FoundGhosts);

  for (AActor *PotentialGhost : FoundGhosts) {
    AGhost *Ghost = Cast<AGhost>(PotentialGhost);
    if (!Ghost) {
      continue;
    }
    Ghosts.Add(Ghost->GetGhostType(), Ghost);
  }
}

AGrid *APacmanGameState::GetGrid() { return Grid; }

AGhost *APacmanGameState::GetGhost(EGhostType GhostType) {
  AGhost **Value = Ghosts.Find(GhostType);

  if (!Value) {
    return nullptr;
  }

  return *Value;
}

void APacmanGameState::RegisterGhostController(AGhostController *GhostController) {
  PublishGhostMode(GhostController);
}
