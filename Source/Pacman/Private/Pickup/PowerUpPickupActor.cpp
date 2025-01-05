#include "Pickup/PowerUpPickupActor.h"

APowerUpPickupActor::APowerUpPickupActor() {
  PrimaryActorTick.bCanEverTick = true;
}

void APowerUpPickupActor::BeginPlay() { Super::BeginPlay(); }

void APowerUpPickupActor::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void APowerUpPickupActor::ApplyAction(AActor *OtherActor) {
  // TODO: Implement possibility to kill ghosts for pacman here
  UE_LOG(LogTemp, Display, TEXT("Pppower UP!"));
}
