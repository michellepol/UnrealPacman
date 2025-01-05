#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"

#include "Grid.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGrid, Log, All);

UCLASS()
class PACMAN_API AGrid : public AActor {
  GENERATED_BODY()

public:
  AGrid();

  virtual void Destroyed() override;

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

  TArray<AActor *> Tiles;
};
