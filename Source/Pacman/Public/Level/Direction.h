#pragma once

#include "UObject/ObjectMacros.h"

UENUM(BlueprintType)
enum class EDirection : uint8 {
  kUp UMETA(DisplayName = "Up"),
  kLeft UMETA(DisplayName = "Left"),
  kRight UMETA(DisplayName = "Right"),
  kDown UMETA(DisplayName = "Down"),
  kUndefined UMETA(DisplayName = "Undefined")
};
