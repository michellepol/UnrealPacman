// Fill out your copyright notice in the Description page of Project Settings.

#include "Geometry/BaseGeometry.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/Blueprint.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"

#include "Math/MathFwd.h"
#include "Templates/Casts.h"

void ABaseGeometry::SpawnPickup() {

  if (!ScorePickupBP || !PowerUpPickupBP) {
    UE_LOG(LogTemp, Error,
           TEXT("ScorePickupBP or PowerUpPickupBP is not defined"));
    return;
  }

  if (!GetWorld()) {
    return;
  }

  FActorSpawnParameters SpawnParams;

  FTransform SpawnTransform;
  SpawnTransform.SetLocation(GetActorLocation());

  switch (PickupType) {
  case EPickupType::Score:
    GetWorld()->SpawnActor<AActor>(ScorePickupBP->GeneratedClass,
                                   SpawnTransform, SpawnParams);
    break;
  case EPickupType::PowerUp:
    GetWorld()->SpawnActor<AActor>(PowerUpPickupBP->GeneratedClass,
                                   SpawnTransform, SpawnParams);
    break;
  }
}

ABaseGeometry::ABaseGeometry() {
  PrimaryActorTick.bCanEverTick = false;

  StaticMeshComponent =
      CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
  SetRootComponent(StaticMeshComponent);
}

// Called when the game starts or when spawned
void ABaseGeometry::BeginPlay() {
  Super::BeginPlay();

  switch (GeometryType) {
  case EGeometryType::Wall:
    StaticMeshComponent->SetStaticMesh(WallMesh);
    break;
  case EGeometryType::Floor:
    StaticMeshComponent->SetStaticMesh(FloorMesh);
    break;
  }

  if (GeometryType == EGeometryType::Floor) {
    SpawnPickup();
  }
}

// Called every frame
void ABaseGeometry::Tick(float DeltaTime) { Super::Tick(DeltaTime); }