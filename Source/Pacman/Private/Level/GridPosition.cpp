#include "Level/GridPosition.h"

bool operator==(const FGridPosition lhs, const FGridPosition rhs) {
  return lhs.row == rhs.row && lhs.col == rhs.col;
}
