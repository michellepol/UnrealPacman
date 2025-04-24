#pragma once

#include "NavigationData.h"

#include "Grid.h"
#include "Tile.h"

struct Node {
  int row, col = 0;
  int g = 0;
  int h = 0; // Эвристическая оценка до цели
  Node *parent = nullptr;

  Node(int row, int col, Node *parent = nullptr)
      : row(row), col(col), parent(parent) {}

  float GetScore() const { return g + h; }

  bool operator==(const Node &other) const {
    return row == other.row && col == other.col;
  }

  bool operator<(const Node &other) const {
    return GetScore() < other.GetScore();
  }
};

FORCEINLINE uint32 GetTypeHash(const Node &node) {
  uint32_t RowHash =
      GetTypeHash(FString::Format(TEXT("r{Row}"), {TEXT("Row"), node.row}));
  uint32_t ColHash =
      GetTypeHash(FString::Format(TEXT("c{Col}"), {TEXT("Col"), node.col}));

  return HashCombine(RowHash, ColHash);
}

struct NodeHash {
  size_t operator()(const Node *node) const { return GetTypeHash(*node); }
};

struct NodeCompare {
    bool operator()(const Node* a, const Node* b) {
        return a->GetScore() > b->GetScore();
    }
};


FPathFindingResult FindPath(ATile *Start, ATile *Goal, AGrid *Grid);

TArray<FGridPosition> AStar(const TArray<TArray<int>> &grid,
                            const FGridPosition start_pos,
                            const FGridPosition goal_pos);