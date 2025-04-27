#include "PacmanCharacter.h"

#include "Utils/Math.h"
#include "Components/SphereComponent.h"

APacmanCharacter::APacmanCharacter() {
    PrimaryActorTick.bCanEverTick = true;
}

void APacmanCharacter::BeginPlay() { Super::BeginPlay(); }

void APacmanCharacter::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

EDirection APacmanCharacter::GetDirection() const {
  // Correctly define directional vectors for Unreal's coordinate system
  const static FVector UpDirection = FVector{0.0f, 1.0f, 0.0f};    // +Y is forward/up
  const static FVector DownDirection = FVector{0.0f, -1.0f, 0.0f}; // -Y is backward/down
  const static FVector RightDirection = FVector{-1.0f, 0.0f, 0.0f}; // -X is right
  const static FVector LeftDirection = FVector{1.0f, 0.0f, 0.0f}; // +X is left

  FVector Velocity = GetVelocity();
  
  // Check if velocity is near-zero to avoid normalization issues
  if (Velocity.SizeSquared() < KINDA_SMALL_NUMBER) {
    return EDirection::kUndefined;
  }
  
  Velocity.Normalize();

  // Use a tolerance for direction comparison
  const float DirectionThreshold = 0.7f; // Cos(45 degrees) ≈ 0.7
  
  // Check if the IsSameDirection function accepts a threshold parameter
  // If it doesn't, we need to modify our approach
  
  // Option 1: If IsSameDirection accepts a threshold parameter
  if (IsSameDirection(UpDirection, Velocity, DirectionThreshold)) {
    return EDirection::kUp;
  } else if (IsSameDirection(DownDirection, Velocity, DirectionThreshold)) {
    return EDirection::kDown;
  } else if (IsSameDirection(RightDirection, Velocity, DirectionThreshold)) {
    return EDirection::kRight;
  } else if (IsSameDirection(LeftDirection, Velocity, DirectionThreshold)) {
    return EDirection::kLeft;
  } else {
    return EDirection::kUndefined;
  }
}
