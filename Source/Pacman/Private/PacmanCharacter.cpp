#include "PacmanCharacter.h"

#include "Components/SphereComponent.h"
#include "Utils/Math.h"

APacmanCharacter::APacmanCharacter() { PrimaryActorTick.bCanEverTick = true; }

void APacmanCharacter::BeginPlay() { Super::BeginPlay(); }

void APacmanCharacter::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

EPacmanDirection APacmanCharacter::GetDirection() const {
  // Correctly define directional vectors for Unreal's coordinate system
  const static FVector UpDirection =
      FVector{0.0f, 1.0f, 0.0f}; // +Y is forward/up
  const static FVector DownDirection =
      FVector{0.0f, -1.0f, 0.0f}; // -Y is backward/down
  const static FVector RightDirection =
      FVector{-1.0f, 0.0f, 0.0f};                                 // -X is right
  const static FVector LeftDirection = FVector{1.0f, 0.0f, 0.0f}; // +X is left

  FVector Velocity = GetVelocity();

  if (Velocity.SizeSquared() < KINDA_SMALL_NUMBER) {
    return EPacmanDirection::kUndefined;
  }

  Velocity.Normalize();

  // Use a tolerance for direction comparison
  const float DirectionThreshold = 0.7f; // Cos(45 degrees) ≈ 0.7

  if (IsSameDirection(UpDirection, Velocity, DirectionThreshold)) {
    return EPacmanDirection::kUp;
  } else if (IsSameDirection(DownDirection, Velocity, DirectionThreshold)) {
    return EPacmanDirection::kDown;
  } else if (IsSameDirection(RightDirection, Velocity, DirectionThreshold)) {
    return EPacmanDirection::kRight;
  } else if (IsSameDirection(LeftDirection, Velocity, DirectionThreshold)) {
    return EPacmanDirection::kLeft;
  } else {
    return EPacmanDirection::kUndefined;
  }
}
