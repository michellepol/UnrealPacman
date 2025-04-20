#include "Utils/Matrix.h"

#include <algorithm>
#include <cmath>

int DiagonalDistance(const int row1, const int col1, const int row2,
                     const int col2) {
  return std::max(std::abs(row1 - row2), std::abs(col1 - col2));
}
