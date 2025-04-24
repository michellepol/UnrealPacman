#pragma once

#include "UObject/ObjectMacros.h"

#include "GridPosition.generated.h"

USTRUCT(BlueprintType)
struct FGridPosition {
  GENERATED_BODY()

  UPROPERTY(BlueprintReadWrite)
  int row = 0;

  UPROPERTY(BlueprintReadWrite)
  int col = 0;
  
  bool operator!=(const FGridPosition &other) const {
    return row != other.row || col != other.col;
  }
};

///@brief Generate hash for Grid position
FORCEINLINE uint32 GetTypeHash(const FGridPosition &Pos) {
  uint32_t RowHash =
      GetTypeHash(FString::Format(TEXT("r{Row}"), {TEXT("Row"), Pos.row}));

  uint32_t ColHash =
      GetTypeHash(FString::Format(TEXT("c{Col}"), {TEXT("Col"), Pos.col}));

  return HashCombine(RowHash, ColHash);
}

struct GridPositionHash {
  size_t operator()(const FGridPosition &pos) const { return GetTypeHash(pos); }
};

bool operator==(const FGridPosition lhs, const FGridPosition rhs);
