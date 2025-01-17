#include "AI/Tasks/Common.h"

#include "AIController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Math/MathFwd.h"

#include "AI/GhostController.h"
#include "AI/Tasks/Common.h"
#include "Level/Tile.h"
#include "PacmanGameState.h"
#include "PacmanPlayer.h"

DEFINE_LOG_CATEGORY(LogTask)

AGhostController *GetGhostController(UBehaviorTreeComponent &OwnerComp) {
  AAIController *AIController = OwnerComp.GetAIOwner();
  if (AIController == nullptr) {
    UE_LOG(LogTask, Error, TEXT("AI Controller is Null"));
    return nullptr;
  }

  return Cast<AGhostController>(AIController);
}

APacmanPlayer *GetPacmanPlayer(UWorld *World) {
  APlayerController *PlayerController =
      UGameplayStatics::GetPlayerController(World, 0);
  if (PlayerController == nullptr) {
    UE_LOG(LogTask, Error, TEXT("Player controller is Null"));
    return nullptr;
  }

  return Cast<APacmanPlayer>(PlayerController->GetPawn());
}

AGrid *GetGrid(UWorld *World) {
  if (!World) {
    UE_LOG(LogTask, Error, TEXT("World is Null"));
    return nullptr;
  }

  AGameStateBase *GameState = World->GetGameState();
  if (!GameState) {
    UE_LOG(LogTask, Error, TEXT("GameState is Null"));
    return nullptr;
  }

  APacmanGameState *PacmanGameState = Cast<APacmanGameState>(GameState);
  if (!PacmanGameState) {
    UE_LOG(LogTask, Error, TEXT("Can't cast BaseGameState to Pacman State"));
    return nullptr;
  }

  return PacmanGameState->GetGrid();
}
