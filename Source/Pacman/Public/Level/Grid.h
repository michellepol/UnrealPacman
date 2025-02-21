#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"

#include "AI/Ghost.h"
#include "Level/Direction.h"
#include "Level/GridPosition.h"

#include "Grid.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGrid, Log, All);

class ATile;

USTRUCT(BlueprintType)
struct FAdjacentTiles {
  GENERATED_BODY()

  ATile **Up;
  ATile **Left;
  ATile **Right;
  ATile **Down;
};

USTRUCT(BlueprintType)
struct FGhostScatterPoint {
  GENERATED_BODY()

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  ATile *Tile = nullptr;
  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  EGhostType GhostType = EGhostType::kRed;
};

UCLASS()
class PACMAN_API AGrid : public AActor {
  GENERATED_BODY()

public:
  AGrid();

  virtual void Destroyed() override;

  virtual void BeginPlay() override;

  ///@brief Get Tile position in Grid by location in World
  UFUNCTION(BlueprintCallable)
  FGridPosition GetTileGridPosition(const int x, const int y) const;

  ///@brief Get Tile by location in World
  UFUNCTION(BlueprintCallable)
  ATile *GetTileByLocation(const int x, const int y) const;

  ///@brief Get Tile by position in Grid
  UFUNCTION(BlueprintCallable)
  ATile *GetTileByGridPos(const FGridPosition pos) const;

  ATile *GetScatterPoint(const EGhostType GhostType) const;

  ///@brief Calculate amount of cells between two cells
  UFUNCTION(BlueprintCallable)
  int CalcTileDistance(const FGridPosition FirstCell,
                       const FGridPosition SecondCell);

  ///@brief Check if current tile is crossroad
  UFUNCTION(BlueprintCallable)
  bool IsCrossRoad(const ATile *Tile, const FAdjacentTiles &AdjacentTiles);

  UFUNCTION(BlueprintCallable)
  FAdjacentTiles GetAdjacentTiles(const ATile *Tile);

public:
  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  int32 Width = 0;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  int32 Length = 0;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  class UBlueprint *TileBP = nullptr;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  int32 TileSize = 0;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  TArray<FGhostScatterPoint> GhostScatterPoints;

private:
  UFUNCTION()
  void OnEditorPlaced(UObject *Object, const TArray<AActor *> &Actors);

  TMap<FGridPosition, ATile *> GridTilesIndex;
};
