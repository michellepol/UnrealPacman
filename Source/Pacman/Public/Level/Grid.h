#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"

#include "AI/Ghost.h"
#include "Level/GridPosition.h"

#include "Grid.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGrid, Log, All);

class ATile;

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
  ATile *GetTile(const FGridPosition pos) const;

  ATile *GetScatterPoint(const EGhostType GhostType) const;

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

  TMap<FGridPosition, AActor *> GridTilesIndex;
};
