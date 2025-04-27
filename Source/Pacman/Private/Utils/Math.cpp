#include "Utils/Math.h"

#include <cmath>

#include "Math/Vector.h"

///@brief Return true if A and B vectors has same direction
bool IsSameDirection(FVector A, FVector B, float Threshold) {
  // Normalize vectors to avoid issues with vector lengths
  A.Normalize();
  B.Normalize();
  
  // Dot product of unit vectors equals cosine of angle between them
  // 1.0 means same direction (0° angle)
  // 0.0 means perpendicular (90° angle)
  // -1.0 means opposite direction (180° angle)
  float DotProduct = FVector::DotProduct(A, B);
  
  // Check if dot product is close to 1.0 within the threshold
  // This tests if vectors point in approximately the same direction
  return DotProduct >= Threshold;
}
