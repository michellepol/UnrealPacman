#include "Pickup/ScorePickupActor.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "PacmanPlayer.h"

AScorePickupActor::AScorePickupActor() { PrimaryActorTick.bCanEverTick = true; }

void AScorePickupActor::BeginPlay() { Super::BeginPlay(); }

void AScorePickupActor::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void AScorePickupActor::ApplyAction(AActor *OtherActor) {
  ACharacter *Character = Cast<APacmanPlayer>(OtherActor);

  if (!Character) {
    UE_LOG(LogTemp, Warning, TEXT("Can't cast overlapped actor to character"));
    return;
  }

  APlayerState *PlayerState = Character->GetPlayerState();

  PlayerState->SetScore(PlayerState->GetScore() + ScoreBonus);
}
