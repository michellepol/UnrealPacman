#include "Pickup/ScorePickupActor.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "PacmanPlayer.h"

// Sets default values
AScorePickupActor::AScorePickupActor() {
  // Set this actor to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AScorePickupActor::BeginPlay() { Super::BeginPlay(); }

// Called every frame
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
