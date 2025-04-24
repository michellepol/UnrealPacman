#include "Level/PathFinding.h"

#include <cmath>
#include <queue>
#include <unordered_map>
#include <vector>

#include "Level/Grid.h"
#include "Level/Tile.h"

#include <math.h>

const std::vector<std::vector<int>> kDirection = {
    // right
    {0, 1},
    // up
    {1, 0},
    // left
    {0, -1},
    // down
    {-1, 0},
};

bool DetectCollision(const int row, const int col,
                     const TArray<TArray<int>> &grid) {
  if (col < 0 || col >= grid[0].Num() || row < 0 || row >= grid.Num() ||
      grid[row][col] == 1) {
    return true;
  }
  return false;
}

int Manhattan(const int source_row, const int source_col, const int target_row,
              const int target_col) {
  int delta_col = abs(source_col - target_col);
  int delta_row = abs(source_row - target_row);
  return delta_col + delta_row;
}

std::vector<Node *>
GetNeigbours(Node *current,
             std::unordered_map<FGridPosition, Node, GridPositionHash> &nodes,
             const TArray<TArray<int>> &grid) {
  std::vector<Node *> neighbors;

  for (int i = 0; i < kDirection.size(); i++) {
    int new_row = current->row + kDirection[i][0];
    int new_col = current->col + kDirection[i][1];

    if (DetectCollision(new_row, new_col, grid)) {
      continue;
    }

    auto neighbor_pos = FGridPosition{new_row, new_col};

    Node *node =
        &nodes.emplace(neighbor_pos, Node(neighbor_pos.row, neighbor_pos.col))
             .first->second;
    neighbors.push_back(node);
  }

  return neighbors;
}

TArray<FGridPosition> AStar(const TArray<TArray<int>> &grid,
                            const FGridPosition start_pos,
                            const FGridPosition goal_pos) {
  static const int kDefaultCost = 1;

  static std::unordered_map<FGridPosition, Node, GridPositionHash> nodes;

  std::priority_queue<Node *, std::vector<Node *>, NodeCompare> frontier;

  // Create or get existing nodes to avoid duplicates
  Node &start = nodes.emplace(start_pos, Node(start_pos.row, start_pos.col))
                    .first->second;
  Node &goal =
      nodes.emplace(goal_pos, Node(goal_pos.row, goal_pos.col)).first->second;
  frontier.push(&start);

  while (!frontier.empty() ) {
    Node *current = frontier.top();
    frontier.pop();

    // Skip nodes too far from goal
    if (Manhattan(current->row, current->col, goal.row, goal.col) > 100) {
      continue;
    }

    if (*current == goal) {
      break;
    }

    auto neighbors = GetNeigbours(current, nodes, grid);

    for (auto *next : neighbors) {
      int new_cost = current->g + kDefaultCost;

      if (next->g == 0 || new_cost < next->g) {
        next->g = new_cost;
        next->h = Manhattan(next->row, next->col, goal.row, goal.col);

        frontier.push(next);
        next->parent = current;
      }
    }
  }

  TArray<FGridPosition> path;

  Node *current = &goal;
  
  // Track visited nodes
  TSet<FGridPosition> visited;

  while (current != nullptr) {
    FGridPosition current_pos = {.row = current->row, .col = current->col};
      // Before processing a node:
      if (visited.Contains({current->row, current->col})) {
        UE_LOG(LogTemp, Warning, TEXT("Visited loop node: %d, %d"), current->row, current->col);
        break;
      }
      visited.Add({current->row, current->col});
      path.Push(current_pos);
      current = current->parent;
  }
  Algo::Reverse(path);


  nodes.clear();

  return path;
}

FPathFindingResult FindPath(ATile *Start, ATile *Goal, AGrid *Grid) {
  if (!Start || !Goal || !Grid) {
    return FPathFindingResult();
  }

  const auto &int_grid = Grid->GetIntGrid();
  FGridPosition start_pos = Start->GetGridPosition();
  FGridPosition goal_pos = Goal->GetGridPosition();

  TArray<FGridPosition> GridPath = AStar(int_grid, start_pos, goal_pos);
  
  TArray<FVector3d> Points;
  for (const auto &GridPos : GridPath) {
    ATile *Tile = Grid->GetTileByGridPosition(GridPos);
    if (!Tile) {
      UE_LOG(LogTemp, Error, TEXT("No Tile in Path"));
      continue;
    }
    Points.Push(Tile->GetActorLocation());
  }

  FPathFindingResult result;
  result.Result = Points.Num() > 1 ? ENavigationQueryResult::Type::Success
                                   : ENavigationQueryResult::Type::Fail;
  result.Path = MakeShared<FNavigationPath>(FNavigationPath(Points));

  return result;
}
