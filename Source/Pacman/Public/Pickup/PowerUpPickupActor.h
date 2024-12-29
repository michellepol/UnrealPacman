#pragma once

#include "CoreMinimal.h"

#include "BasePickupActor.h"

#include "PowerUpPickupActor.generated.h"

UCLASS()
class PACMAN_API APowerUpPickupActor : public ABasePickupActor {
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  APowerUpPickupActor();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  void ApplyAction(AActor *OtherActor) override;

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;
};
