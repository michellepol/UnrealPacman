// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Templates/Casts.h"
#include "UObject/ObjectMacros.h"

#include "BaseGeometry.generated.h"

UENUM(BlueprintType)
enum class EGeometryType : uint8 { Wall = 0, Floor = 1 };

UENUM(BlueprintType)
enum class EPickupType : uint8 { Score = 0, PowerUp = 1 };

UCLASS()
class PACMAN_API ABaseGeometry : public AActor {
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  ABaseGeometry();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  virtual void OnConstruction(const FTransform &Transform) override;

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  void SpawnPickup();

  // Components
  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  class UStaticMeshComponent *StaticMeshComponent = nullptr;

  // Properties
  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  class UStaticMesh *FloorMesh = nullptr;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  class UStaticMesh *WallMesh = nullptr;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  class UBlueprint *PowerUpPickupBP = nullptr;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  class UBlueprint *ScorePickupBP = nullptr;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  EGeometryType GeometryType;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  EPickupType PickupType;

private:
  void SetStaticMeshByGeometryType();
};
