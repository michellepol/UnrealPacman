#include "PacmanCharacter.h"

#include "Utils/Math.h"

APacmanCharacter::APacmanCharacter() { PrimaryActorTick.bCanEverTick = true; }

void APacmanCharacter::BeginPlay() { Super::BeginPlay(); }

void APacmanCharacter::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

EDirection APacmanCharacter::GetDirection() const {
  const static FVector UpDirection = FVector{0.0f, 1.0f, 0.0f};
  const static FVector DownDirection = FVector{0.0f, -1.0f, 0.0f};
  const static FVector RightDirection = FVector{-1.0f, 0.0f, 0.0f};
  const static FVector LeftDirection = FVector{1.0f, 0.0f, 0.0f};

  FVector Velocity = GetVelocity();
  Velocity.Normalize();

  if (IsSameDirection(UpDirection, Velocity)) {
    return EDirection::kUp;
  } else if (IsSameDirection(DownDirection, Velocity)) {
    return EDirection::kDown;
  } else if (IsSameDirection(RightDirection, Velocity)) {
    return EDirection::kRight;
  } else if (IsSameDirection(LeftDirection, Velocity)) {
    return EDirection::kLeft;
  } else {
    return EDirection::kUndefined;
  }
}
