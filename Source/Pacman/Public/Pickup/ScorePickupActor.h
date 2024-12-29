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

  // Called every frame
  virtual void Tick(float DeltaTime) override;

  UPROPERTY(EditAnywhere)
  float ScoreBonus = 10.0f;

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  void ApplyAction(AActor *OtherActor) override;
};
