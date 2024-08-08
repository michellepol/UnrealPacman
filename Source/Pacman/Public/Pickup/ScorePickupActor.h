#pragma once

#include "CoreMinimal.h"

#include "BasePickupActor.h"

#include "ScorePickupActor.generated.h"

UCLASS()
class PACMAN_API AScorePickupActor : public ABasePickupActor {
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  AScorePickupActor();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;
};
