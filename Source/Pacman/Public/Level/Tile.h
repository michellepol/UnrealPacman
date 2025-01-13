// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Templates/Casts.h"
#include "UObject/ObjectMacros.h"

#include "Level/GridPosition.h"

#include "Tile.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTile, Log, All);

UENUM(BlueprintType) enum class ETileType : uint8 { Wall = 0, Floor = 1 };

UENUM(BlueprintType)
enum class EPickupType : uint8 { Score = 0, PowerUp = 1, None = 2 };

UCLASS()
class PACMAN_API ATile : public AActor {
  GENERATED_BODY()

public:
  ATile();

  void SetDebugMaterial();

  void SetGridPosition(const FGridPosition NewGridPosition);

  FGridPosition GetGridPosition() const;

protected:
  virtual void BeginPlay() override;

  virtual void OnConstruction(const FTransform &Transform) override;

public:
  virtual void Tick(float DeltaTime) override;

  // Components
  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  class UStaticMeshComponent *StaticMeshComponent = nullptr;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  class UMaterialInterface *DebugMaterial = nullptr;

  // Geometry
  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  class UStaticMesh *WallMesh = nullptr;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  class UMaterialInterface *WallMaterial = nullptr;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  class UStaticMesh *FloorMesh = nullptr;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  class UMaterialInterface *FloorMaterial = nullptr;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  ETileType TileType = ETileType::Floor;

  // Pickup actors
  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  class UBlueprint *PowerUpPickupBP = nullptr;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  class UBlueprint *ScorePickupBP = nullptr;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  EPickupType PickupType = EPickupType::Score;

private:
  void SetStaticMeshByTileType();

  void SpawnPickup();

  FGridPosition GridPosition;
};
