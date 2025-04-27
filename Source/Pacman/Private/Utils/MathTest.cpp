#include "Misc/AutomationTest.h"

#include <Utils/Math.h>

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FIsSameDirection,
                                 "Pacman.Tests.IsSameDirection",
                                 EAutomationTestFlags::EditorContext |
                                     EAutomationTestFlags::EngineFilter)

bool FIsSameDirection::RunTest(const FString &Parameters) {
  TestTrue("+X Same Direction", IsSameDirection(FVector{1.0f, 0.0f, 0.0f},
                                                FVector{1.0f, 0.0f, 0.0f}));

  TestTrue("-X Same Direction", IsSameDirection(FVector{-1.0f, 0.0f, 0.0f},
                                                FVector{-1.0f, 0.0f, 0.0f}));

  TestTrue("+Y Same Direction", IsSameDirection(FVector{0.0f, 1.0f, 0.0f},
                                                FVector{0.0f, 1.0f, 0.0f}));

  TestTrue("-Y Same Direction", IsSameDirection(FVector{0.0f, 1.0f, 0.0f},
                                                FVector{0.0f, 1.0f, 0.0f}));

  TestFalse(
      "-X Different Direction",
      IsSameDirection(FVector{-1.0f, 0.0f, 0.0f}, FVector{1.0f, 0.0f, 0.0f}));

  TestFalse(
      "+X Different Direction",
      IsSameDirection(FVector{1.0f, 0.0f, 0.0f}, FVector{-1.0f, 0.0f, 0.0f}));

  TestFalse(
      "+Y Different Direction",
      IsSameDirection(FVector{0.0f, 1.0f, 0.0f}, FVector{0.0f, -1.0f, 0.0f}));

  TestFalse(
      "-Y Different Direction",
      IsSameDirection(FVector{0.0f, -1.0f, 0.0f}, FVector{0.0f, 1.0f, 0.0f}));

  TestFalse("X Close Direction", IsSameDirection(FVector{1.0f, 0.1f, 0.0f},
                                                 FVector{1.0f, 0.0f, 0.0f}));

  TestFalse("Y Close Direction", IsSameDirection(FVector{0.1f, 1.0f, 0.0f},
                                                 FVector{0.0f, 1.0f, 0.0f}));

  return true;
}
