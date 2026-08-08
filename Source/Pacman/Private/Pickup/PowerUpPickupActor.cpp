#include "Pickup/PowerUpPickupActor.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "PacmanGameState.h"
#include "PacmanPlayer.h"

APowerUpPickupActor::APowerUpPickupActor() {
  PrimaryActorTick.bCanEverTick = true;
}

void APowerUpPickupActor::BeginPlay() { Super::BeginPlay(); }

void APowerUpPickupActor::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void APowerUpPickupActor::ApplyAction(AActor *OtherActor) {
  ACharacter *Character = Cast<APacmanPlayer>(OtherActor);

  if (!Character) {
    UE_LOG(LogTemp, Warning, TEXT("Can't cast overlapped actor to character"));
    return;
  }

  APlayerState *PlayerState = Character->GetPlayerState();
  if (!PlayerState) {
    UE_LOG(LogTemp, Warning, TEXT("PlayerState is not ready"));
    return;
  }

  PlayerState->SetScore(PlayerState->GetScore() + ScoreBonus);

  if (APacmanGameState *GameState =
          Cast<APacmanGameState>(GetWorld()->GetGameState())) {
    GameState->StartFrightenedMode(FrightenedDuration);
  }
}
