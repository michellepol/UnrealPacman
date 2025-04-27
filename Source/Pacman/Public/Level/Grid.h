#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"

#include "AI/Ghost.h"
#include "Level/Direction.h"
#include "Level/GridPosition.h"

#include "Grid.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGrid, Log, All);

class ATile;

USTRUCT(BlueprintType)
struct FAdjacentTiles {
  GENERATED_BODY()

  ATile *Up = nullptr;
  ATile *Left = nullptr;
  ATile *Right = nullptr;
  ATile *Down = nullptr;

  TArray<ATile * > ToArray() const {
    TArray<ATile* > PtrArray;
    PtrArray.SetNum(4);
    PtrArray[0] = Up;
    PtrArray[1] = Left;
    PtrArray[2] = Right;
    PtrArray[3] = Down;
    return PtrArray;
  }
};

UCLASS()
class PACMAN_API AGrid : public AActor {
  GENERATED_BODY()

public:
  AGrid();

  virtual void Destroyed() override;

  virtual void BeginPlay() override;

  FGridPosition GetTileGridPosition(const FVector WorldLocation) const;

  ///@brief Get Tile by position in Grid
  UFUNCTION(BlueprintCallable)
  ATile *GetTileByGridPosition(const FGridPosition GridPosition) const;

  ///@brief Get Tile by location in World
  UFUNCTION(BlueprintCallable)
  ATile *GetTileByLocation(const FVector WorldLocation) const;

  ///@brief Check if current tile is crossroad
  UFUNCTION(BlueprintCallable)
  bool IsCrossRoad(const ATile *Tile, const FAdjacentTiles &AdjacentTiles);

  UFUNCTION(BlueprintCallable)
  FAdjacentTiles GetAdjacentTiles(const ATile *Tile) const;

  TArray<TArray<int>> ToIntGrid();

  const TArray<TArray<int>> &GetIntGrid() const;

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

  TMap<FGridPosition, ATile *> GridTilesIndex;

  TArray<TArray<int>> GridInt;
};
