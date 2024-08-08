#include "Pickup/ScorePickupActor.h"

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
