#pragma once

#include "Containers/Array.h"
#include "Containers/Map.h"
#include "CoreMinimal.h"

#include "Level/GridPosition.h"

class ATile;

///@brief Find the nearest tile in the grid that satisfies a predicate.
///
///@details Performs a breadth-first search starting from Start position and
/// returns the first tile for which Predicate returns true. If no such tile is
/// found within grid bounds, returns nullptr.
ATile *FindNearestTile(const TMap<FGridPosition, ATile *> &GridTiles,
                        const FGridPosition &Start, const int32 Width,
                        const int32 Length,
                        TFunction<bool(const ATile *)> Predicate);