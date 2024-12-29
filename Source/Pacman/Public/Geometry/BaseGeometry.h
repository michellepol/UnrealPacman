// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Templates/Casts.h"
#include "UObject/ObjectMacros.h"

#include "BaseGeometry.generated.h"

UENUM()
enum class EGeometryType { Wall = 0, Floor = 1 };

UENUM()
enum class EPickupType { Score = 0, PowerUp = 1 };

UCLASS()
class PACMAN_API ABaseGeometry : public AActor {
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  ABaseGeometry();

protected:
  virtual void BeginPlay() override;

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  void SpawnPickup();

  // Components

  UPROPERTY(EditAnywhere)
  class UStaticMeshComponent *StaticMeshComponent = nullptr;

  // Properties

  UPROPERTY(EditAnywhere)
  class UStaticMesh *FloorMesh = nullptr;

  UPROPERTY(EditAnywhere)
  class UStaticMesh *WallMesh = nullptr;

  UPROPERTY(EditAnywhere)
  class UBlueprint *PowerUpPickupBP = nullptr;

  UPROPERTY(EditAnywhere)
  class UBlueprint *ScorePickupBP = nullptr;

  UPROPERTY(EditAnywhere)
  EGeometryType GeometryType;

  UPROPERTY(EditAnywhere)
  EPickupType PickupType;
};
