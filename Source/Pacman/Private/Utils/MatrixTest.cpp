#include "Misc/AutomationTest.h"

#include <Utils/Matrix.h>

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDiagonalDistance,
                                 "Pacman.Tests.DiagonalDistance",
                                 EAutomationTestFlags::EditorContext |
                                     EAutomationTestFlags::EngineFilter)

bool FDiagonalDistance::RunTest(const FString &Parameters) {
  // Тест 1: Точки находятся в одной и той же позиции
  TestTrue("SamePosition", DiagonalDistance(0, 0, 0, 0) == 0);

  // Тест 2: Точки находятся на одной строке
  TestTrue("SameRow", DiagonalDistance(0, 0, 0, 5) == 5);

  // Тест 3: Точки находятся на одном столбце
  TestTrue("SameColumn", DiagonalDistance(0, 0, 5, 0) == 5);

  // Тест 4: Точки находятся на диагонали
  TestTrue("Diagonal", DiagonalDistance(0, 0, 5, 5) == 5);

  // Тест 5: Точки находятся на диагонали, но в разных направлениях
  TestTrue("ReverseDiagonal", DiagonalDistance(0, 5, 5, 0) == 5);

  // Тест 6: Точки находятся в разных строках и столбцах, но не на диагонали
  TestTrue("Test6", DiagonalDistance(0, 0, 3, 4) == 4);

  // Тест 7: Точки находятся в разных строках и столбцах, но не на диагонали
  TestTrue("Test7", DiagonalDistance(0, 0, 4, 3) == 4);

  // Тест 8: Точки находятся в разных строках и столбцах, но не на диагонали
  TestTrue("Test8", DiagonalDistance(1, 2, 4, 6) == 4);

  // Тест 9: Точки находятся в разных строках и столбцах, но не на диагонали
  TestTrue("Test9", DiagonalDistance(5, 5, 1, 1) == 4);

  // Тест 10: Точки находятся в разных строках и столбцах, но не на диагонали
  TestTrue("Test10", DiagonalDistance(7, 3, 3, 7) == 4);

  return true;
}
