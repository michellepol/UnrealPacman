#include "Utils/GridUtils.h"

#include "Containers/Queue.h"
#include "Containers/Set.h"

#include "Level/Tile.h"

ATile *FindNearestTile(const TMap<FGridPosition, ATile *> &GridTiles,
                        const FGridPosition &Start, const int32 Width,
                        const int32 Length,
                        TFunction<bool(const ATile *)> Predicate) {
  if (Start.row < 0 || Start.row >= Width || Start.col < 0 ||
      Start.col >= Length) {
    return nullptr;
  }

  if (ATile *const *StartTilePtr = GridTiles.Find(Start)) {
    if (*StartTilePtr && Predicate(*StartTilePtr)) {
      return *StartTilePtr;
    }
  }

  TSet<FGridPosition> Visited;
  TQueue<FGridPosition> Queue;

  Queue.Enqueue(Start);
  Visited.Add(Start);

  const TArray<FGridPosition> Directions = {
      FGridPosition{1, 0}, FGridPosition{-1, 0},
      FGridPosition{0, 1}, FGridPosition{0, -1}};

  while (!Queue.IsEmpty()) {
    FGridPosition Current;
    Queue.Dequeue(Current);

    for (const FGridPosition &Dir : Directions) {
      FGridPosition Neighbor{Current.row + Dir.row, Current.col + Dir.col};

      if (Neighbor.row < 0 || Neighbor.row >= Width || Neighbor.col < 0 ||
          Neighbor.col >= Length) {
        continue;
      }

      if (Visited.Contains(Neighbor)) {
        continue;
      }

      Visited.Add(Neighbor);

      if (ATile *const *TilePtr = GridTiles.Find(Neighbor)) {
        if (*TilePtr && Predicate(*TilePtr)) {
          return *TilePtr;
        }
      }

      Queue.Enqueue(Neighbor);
    }
  }

  return nullptr;
}