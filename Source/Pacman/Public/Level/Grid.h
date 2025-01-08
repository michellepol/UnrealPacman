#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"

#include "Level/GridPosition.h"

#include "Grid.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGrid, Log, All);

class ATile;

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
