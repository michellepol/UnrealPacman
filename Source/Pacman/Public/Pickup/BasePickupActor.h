// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/Actor.h"

#include "BasePickupActor.generated.h"

UCLASS()
class PACMAN_API ABasePickupActor : public AActor {
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  ABasePickupActor();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  UFUNCTION()
  void OnBeginOverlap(UPrimitiveComponent *OverlappedComponent,
                      AActor *OtherActor, UPrimitiveComponent *OtherComp,
                      int32 OtherBodyIndex, bool bFromSweep,
                      const FHitResult &SweepResult);

  virtual void ApplyAction(AActor *OtherActor);

  class USphereComponent *CollisionComponent = nullptr;

  UPROPERTY(EditDefaultsOnly)
  class UStaticMeshComponent *StaticMeshComponent = nullptr;

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  UPROPERTY(EditAnywhere)
  float Scale = 0.8f;
};
