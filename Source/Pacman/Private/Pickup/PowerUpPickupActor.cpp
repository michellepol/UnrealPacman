#include "Pickup/PowerUpPickupActor.h"

// Sets default values
APowerUpPickupActor::APowerUpPickupActor() {
  // Set this actor to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APowerUpPickupActor::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void APowerUpPickupActor::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void APowerUpPickupActor::ApplyAction(AActor *OtherActor) {
  UE_LOG(LogTemp, Warning, TEXT("Pppower UP!"));
}
