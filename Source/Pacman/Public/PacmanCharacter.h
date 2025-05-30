#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Level/Direction.h"

#include "PacmanCharacter.generated.h"

UCLASS()
class PACMAN_API APacmanCharacter : public ACharacter {
  GENERATED_BODY()

public:
  APacmanCharacter();

  ///@brief Calculate direction of character in world
  EDirection GetDirection() const;

protected:
  virtual void BeginPlay() override;

public:
  virtual void Tick(float DeltaTime) override;
};
