#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "PacmanCharacter.generated.h"

UENUM(BlueprintType)
enum class EDirection : uint8 {
  kUp UMETA(DisplayName = "Up"),
  kLeft UMETA(DisplayName = "Left"),
  kRight UMETA(DisplayName = "Right"),
  kDown UMETA(DisplayName = "Down"),
  kUndefined UMETA(DisplayName = "Undefined")
};

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
