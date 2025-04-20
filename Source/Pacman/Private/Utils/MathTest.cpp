#include "Misc/AutomationTest.h"

#include <Utils/Math.h>

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FIsClose, "Pacman.Tests.IsClose",
                                 EAutomationTestFlags::EditorContext |
                                     EAutomationTestFlags::EngineFilter)

bool FIsClose::RunTest(const FString &Parameters) {
  // different values
  TestTrue("Same value", IsClose(0.0001f, 0.0001f, 0.0001f));
  TestTrue("Different value", IsClose(0.0001f, 0.0002f, 0.0001f));
  TestFalse("Different value more than tolerance",
            IsClose(0.0001f, 0.0003f, 0.0001f));

  // different tolerance
  TestFalse("Big tolerance", IsClose(0.00014f, 0.00012f, 0.00001f));
  TestTrue("Small tolerance", IsClose(0.00014f, 0.00012f, 0.0001f));
  return true;
}

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
