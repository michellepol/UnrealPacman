// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Ghost.h"

#include "Components/SceneComponent.h"
#include "NavModifierVolume.h"
#include "NavigationSystem.h"

AGhost::AGhost() { PrimaryActorTick.bCanEverTick = true; }

void AGhost::BeginPlay() {
  Super::BeginPlay();

  // ANavModifierVolume needs authored brush geometry to provide valid nav
  // bounds. Spawning it at runtime creates an empty volume in UE 5.8 (and the
  // navigation system reports it as invalid). Ghost movement uses the custom
  // grid pathfinder, so no runtime navigation volume is required here.
}

void AGhost::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  if (NavModifierVolume) {
    FVector Location = GetActorLocation();
    Location.Z -= 60;
    NavModifierVolume->SetActorLocation(Location);
  }
}

EGhostType AGhost::GetGhostType() { return GhostType; }
