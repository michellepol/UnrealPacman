#pragma once

#include <Math/MathFwd.h>

///@brief Return true if A and B is equal with precision
bool IsClose(float A, float B, float Tolerance = 0.0001f);

///@brief Return true if A and B vectors has same direction
bool IsSameDirection(FVector A, FVector B);
