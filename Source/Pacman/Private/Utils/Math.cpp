#include "Utils/Math.h"

#include <cmath>

#include "Math/Vector.h"

bool IsClose(float A, float B, float Tolerance) {
  return std::abs(A - B) <= Tolerance;
}

bool IsSameDirection(FVector A, FVector B) {
  float DotProduct = FVector::DotProduct(A, B);

  float LengthProduct = A.Length() * B.Length();

  // dot product of vectors with same direction is equal to multiply of their
  // length
  return IsClose(DotProduct, LengthProduct);
}
