// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Ghost.h"

#include "NavModifierVolume.h"
#include "NavigationSystem.h"

AGhost::AGhost() { PrimaryActorTick.bCanEverTick = true; }

void AGhost::BeginPlay() {
  Super::BeginPlay();

  const static FVector Extent = FVector(100.0f, 100.0f, 100.0f);

  if (NavModifierVolumeClass && NavAreaClass) {
    UE_LOG(LogTemp, Display, TEXT("Spawn Navigation Volume"));

    NavModifierVolume = GetWorld()->SpawnActor<ANavModifierVolume>(
        NavModifierVolumeClass, GetActorLocation(), FRotator::ZeroRotator);

    if (NavModifierVolume) {
      NavModifierVolume->SetActorRelativeScale3D(Extent);
      NavModifierVolume->SetAreaClass(NavAreaClass);

      FAttachmentTransformRules AttachmentRules(
          EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
          EAttachmentRule::KeepWorld, false);

      NavModifierVolume->AttachToActor(this, AttachmentRules);
      NavModifierVolume->SetLockLocation(false);
    }
  }
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
