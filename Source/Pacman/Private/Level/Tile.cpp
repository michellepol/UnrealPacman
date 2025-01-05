// Fill out your copyright notice in the Description page of Project Settings.

#include "Level/Tile.h"

#include "Engine/Blueprint.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"

#include "Components/StaticMeshComponent.h"

#include "Math/MathFwd.h"
#include "Templates/Casts.h"

DEFINE_LOG_CATEGORY(LogTile);

ATile::ATile() {
  PrimaryActorTick.bCanEverTick = false;

  StaticMeshComponent =
      CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
  SetRootComponent(StaticMeshComponent);
}

void ATile::BeginPlay() {
  Super::BeginPlay();

  if (TileType == ETileType::Floor && PickupType != EPickupType::None) {
    SpawnPickup();
  }
}

void ATile::OnConstruction(const FTransform &Transform) {
  Super::OnConstruction(Transform);

  SetStaticMeshByTileType();
}

void ATile::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void ATile::SetStaticMeshByTileType() {
  switch (TileType) {
  case ETileType::Wall:
    StaticMeshComponent->SetStaticMesh(WallMesh);
    StaticMeshComponent->SetMaterial(0, WallMaterial);
    break;
  case ETileType::Floor:
    StaticMeshComponent->SetStaticMesh(FloorMesh);
    StaticMeshComponent->SetMaterial(0, FloorMaterial);
    break;
  }
}

void ATile::SpawnPickup() {
  if (!ScorePickupBP || !PowerUpPickupBP) {
    UE_LOG(LogTile, Error,
           TEXT("ScorePickupBP or PowerUpPickupBP is not defined"));
    return;
  }

  if (!GetWorld()) {
    return;
  }

  auto *World = GetWorld();

  FActorSpawnParameters SpawnParams;

  FTransform SpawnTransform;
  SpawnTransform.SetLocation(GetActorLocation());

  switch (PickupType) {
  case EPickupType::Score:
    World->SpawnActor<AActor>(ScorePickupBP->GeneratedClass, SpawnTransform,
                              SpawnParams);
    break;
  case EPickupType::PowerUp:
    World->SpawnActor<AActor>(PowerUpPickupBP->GeneratedClass, SpawnTransform,
                              SpawnParams);
    break;
  }
}
