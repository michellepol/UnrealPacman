// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup/BasePickupActor.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/StaticMesh.h"

ABasePickupActor::ABasePickupActor() {
  PrimaryActorTick.bCanEverTick = true;

  CollisionComponent =
      CreateDefaultSubobject<USphereComponent>("CollisionComponent");
  SetRootComponent(CollisionComponent);

  CollisionComponent->SetSphereRadius(115);
  CollisionComponent->SetGenerateOverlapEvents(true);
  CollisionComponent->OnComponentBeginOverlap.AddDynamic(
      this, &ABasePickupActor::OnBeginOverlap);

  CollisionComponent->SetCollisionResponseToAllChannels(
      ECollisionResponse::ECR_Ignore);

  CollisionComponent->SetCollisionResponseToChannel(
      ECollisionChannel::ECC_GameTraceChannel1,
      ECollisionResponse::ECR_Overlap);

  StaticMeshComponent =
      CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
  StaticMeshComponent->SetWorldScale3D(FVector(Scale));
  StaticMeshComponent->SetupAttachment(CollisionComponent);
  StaticMeshComponent->SetCollisionProfileName(FName("OverlapAllDynamic"));
}

void ABasePickupActor::BeginPlay() {
  Super::BeginPlay();

  check(CollisionComponent);
  check(StaticMeshComponent);
}

void ABasePickupActor::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void ABasePickupActor::OnBeginOverlap(UPrimitiveComponent *OverlappedComponent,
                                      AActor *OtherActor,
                                      UPrimitiveComponent *OtherComp,
                                      int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult &SweepResult) {
  ApplyAction(OtherActor);

  Destroy();
}

void ABasePickupActor::ApplyAction(AActor *OtherActor) {
  UE_LOG(LogTemp, Display, TEXT("Base pickup applied action"));
}
