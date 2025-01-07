#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"

#include "Grid.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGrid, Log, All);

class ATile;

USTRUCT(BlueprintType)
struct FGridPosition {
  GENERATED_BODY()

  UPROPERTY(BlueprintReadWrite)
  int row = 0;

  UPROPERTY(BlueprintReadWrite)
  int col = 0;
};

FORCEINLINE uint32 GetTypeHash(const FGridPosition &Pos) {
  // Use a combination of the vector components to generate a hash

  uint32_t RowHash =
      GetTypeHash(FString::Format(TEXT("r{Row}"), {TEXT("Row"), Pos.row}));

  uint32_t ColHash =
      GetTypeHash(FString::Format(TEXT("r{Col}"), {TEXT("Col"), Pos.col}));

  return HashCombine(RowHash, ColHash);
}

bool operator==(const FGridPosition lhs, const FGridPosition rhs);

UCLASS()
class PACMAN_API AGrid : public AActor {
  GENERATED_BODY()

public:
  AGrid();

  virtual void Destroyed() override;

  virtual void BeginPlay() override;

  UFUNCTION(BlueprintCallable)
  FGridPosition GetTileGridPosByLocation(const int x, const int y) const;

  UFUNCTION(BlueprintCallable)
  ATile *GetTileByLocation(const int x, const int y) const;

  UFUNCTION(BlueprintCallable)
  ATile *GetTileByGridPos(const FGridPosition pos) const;

public:
  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  int32 Width = 0;
  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  int32 Length = 0;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  class UBlueprint *TileBP = nullptr;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  int32 TileSize = 0;

private:
  UFUNCTION()
  void OnEditorPlaced(UObject *Object, const TArray<AActor *> &Actors);

  TMap<FGridPosition, AActor *> GridTilesIndex;
};
