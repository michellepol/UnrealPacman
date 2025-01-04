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

void ABaseGeometry::SetStaticMeshByGeometryType() {
  switch (GeometryType) {
  case EGeometryType::Wall:
    StaticMeshComponent->SetStaticMesh(WallMesh);
    break;
  default:
    return;
  }
}

void ABaseGeometry::BeginPlay() {
  Super::BeginPlay();

  SetStaticMeshByGeometryType();

  if (GeometryType == EGeometryType::Floor) {
    SpawnPickup();
  }
}

void ABaseGeometry::OnConstruction(const FTransform &Transform) {
  SetStaticMeshByGeometryType();
}

void ABaseGeometry::Tick(float DeltaTime) { Super::Tick(DeltaTime); }
