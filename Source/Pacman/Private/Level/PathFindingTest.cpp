#include "Misc/AutomationTest.h"

#include "Level/PathFinding.h"

#include <vector>

IMPLEMENT_SIMPLE_AUTOMATION_TEST(AStarTest, "Pacman.Tests.AStarTest",
                                 EAutomationTestFlags::EditorContext |
                                     EAutomationTestFlags::EngineFilter)

bool AStarTest::RunTest(const FString &Parameters) {

  // Пример grid map
  TArray<TArray<int>> grid = {{0, 1, 0, 0, 0},
                              {0, 1, 0, 1, 0},
                              {0, 0, 0, 1, 0},
                              {0, 1, 1, 1, 0},
                              {0, 0, 0, 0, 0}};

  // Тест 1: Путь существует
  {
    FGridPosition start = FGridPosition(0, 0);
    FGridPosition goal = FGridPosition(4, 4);
    auto path = AStar(grid, start, goal);
    TestTrue("PathExist", !path.IsEmpty()); // Ожидаем, что путь найден
  }

  // Тест 2: Путь не существует (начальная и конечная точки заблокированы)
  {
    FGridPosition start = FGridPosition(0, 0);
    FGridPosition goal = FGridPosition(4, 4);
    grid[4][4] = 1; // Блокируем конечную точку
    auto path = AStar(grid, start, goal);
    TestTrue("NoPath", path.IsEmpty()); // Ожидаем, что путь не найден
    grid[4][4] = 0;                     // Восстанавливаем grid
  }

  // Тест 3: Начальная и конечная точки совпадают
  {
    FGridPosition start = FGridPosition(2, 2);
    FGridPosition goal = FGridPosition(2, 2);
    auto path = AStar(grid, start, goal);
    TestTrue("Start=Target",
             !path.IsEmpty()); // Ожидаем, что путь найден (начальная и конечная
                               // точки совпадают)
    TestTrue("Start=Target",
             path.Num() == 1); // Путь должен состоять из одной точки
  }

  // Тест 4: Начальная точка заблокирована
  {
    FGridPosition start = FGridPosition(0, 0);
    FGridPosition goal = FGridPosition(4, 4);
    grid[0][0] = 1; // Блокируем начальную точку
    auto path = AStar(grid, start, goal);
    TestTrue("NoGrid", path.IsEmpty()); // Ожидаем, что путь не найден
    grid[0][0] = 0;                     // Восстанавливаем grid
  }

  // Тест 5: Путь через узкое место
  {
    TArray<TArray<int>> narrow_grid = {{0, 0, 0, 1, 0},
                                                 {1, 1, 0, 1, 0},
                                                 {0, 0, 0, 1, 0},
                                                 {0, 1, 1, 1, 0},
                                                 {0, 0, 0, 0, 0}};
    FGridPosition start = FGridPosition(0, 0);
    FGridPosition goal = FGridPosition(4, 4);
    auto path = AStar(narrow_grid, start, goal);
    TestTrue("Narrow", !path.IsEmpty()); // Ожидаем, что путь найден
  }

  // Тест 6: Путь через лабиринт
  {
    TArray<TArray<int>> maze_grid = {{0, 1, 0, 0, 0},
                                               {0, 1, 0, 1, 0},
                                               {0, 0, 0, 1, 0},
                                               {0, 1, 1, 1, 0},
                                               {0, 0, 0, 0, 0}};
    FGridPosition start = FGridPosition(0, 0);
    FGridPosition goal = FGridPosition(4, 4);
    auto path = AStar(maze_grid, start, goal);
    TestTrue("Maze", !path.IsEmpty()); // Ожидаем, что путь найден
  }

   return 0;
}
