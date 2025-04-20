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
                     const std::vector<std::vector<int>> &grid) {
  if (col < 0 || col >= grid[0].size() || row < 0 || row >= grid.size() ||
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

std::vector<Node *> GetNeigbours(Node *current,
                                 TMap<FGridPosition, Node *> &nodes,
                                 const std::vector<std::vector<int>> &grid) {
  std::vector<Node *> neighbors;

  for (int i = 0; i < kDirection.size(); i++) {
    int new_row = current->row + kDirection[i][0];
    int new_col = current->col + kDirection[i][1];

    if (DetectCollision(new_row, new_col, grid)) {
      continue;
    }

    auto neighbor_pos = FGridPosition{new_row, new_col};

    Node **node_ptr = nodes.Find(neighbor_pos);
    if (node_ptr) {
      neighbors.push_back(*node_ptr);
    } else {
      Node *new_node = new Node(neighbor_pos.row, neighbor_pos.col);
      neighbors.push_back(new_node);
      nodes.Add(neighbor_pos, new_node);
    }
  }

  return neighbors;
}

TArray<FGridPosition> AStar(const std::vector<std::vector<int>> &grid,
                            const FGridPosition start_pos,
                            const FGridPosition goal_pos) {
  // Создаем начальный и целевой узлы
  Node *start = new Node(start_pos.row, start_pos.col);
  Node *goal = new Node(goal_pos.row, goal_pos.col);

  static const int kDefaultCost = 1;

  std::priority_queue<Node *> frontier;
  TMap<FGridPosition, Node *> nodes = {{start_pos, start}, {goal_pos, goal}};
  frontier.push(start);

  int distance = 0;

  while (!frontier.empty()) {

    if (distance > 1000) {
      break;
    }
    distance++;

    auto *current = frontier.top();
    frontier.pop();

    if (current == goal) {
      goal->parent = current->parent;
      break;
    }

    auto neighbors = GetNeigbours(current, nodes, grid);

    for (auto *next : neighbors) {
      int new_cost = current->g + kDefaultCost;

      if (next->g == 0 || new_cost < next->g) {
        next->g = new_cost;
        next->h = Manhattan(next->row, next->col, goal->row, goal->col);

        frontier.push(next);
        next->parent = current;
      }
    }
  }

  TArray<FGridPosition> path;

  Node *current = goal;

  while (current != nullptr) {
    FGridPosition current_pos = {.row = current->row, .col = current->col};
    path.Push(current_pos);
    current = current->parent;
  }
  Algo::Reverse(path);

  for (auto &[pos, node] : nodes) {
    delete node;
  }

  UE_LOG(LogTemp, Error, TEXT("Found Path, %i"), path.Num());

  return path;
}

FPathFindingResult FindPath(ATile *Start, ATile *Goal, AGrid *Grid) {
  if (!Start || !Goal || !Grid) {
    return FPathFindingResult();
  }

  // TODO: optimize
  auto grid = Grid->ToIntGrid();
  FGridPosition start_pos = Start->GetGridPosition();
  FGridPosition goal_pos = Goal->GetGridPosition();

  TArray<FGridPosition> GridPath = AStar(grid, start_pos, goal_pos);

  TArray<FVector3d> Points;
  for (const auto &GridPos : GridPath) {
    ATile *Tile = Grid->GetTileByGridPosition(GridPos);
    if (!Tile) {
      UE_LOG(LogTemp, Error, TEXT("No Tile in Path"));
      continue;
    }
    Tile->SetDebugMaterial();
    Points.Push(Tile->GetActorLocation());
  }

  FPathFindingResult result;
  result.Result = Points.Num() > 1 ? ENavigationQueryResult::Type::Success
                                   : ENavigationQueryResult::Type::Fail;
  result.Path = MakeShared<FNavigationPath>(FNavigationPath(Points));

  return result;
}
