// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup/BasePickupActor.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/StaticMesh.h"

// Sets default values
ABasePickupActor::ABasePickupActor() {
  // Set this actor to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
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
}

// Called when the game starts or when spawned
void ABasePickupActor::BeginPlay() {
  Super::BeginPlay();

  check(CollisionComponent);
  check(StaticMeshComponent);
}

// Called every frame
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
  UE_LOG(LogTemp, Warning, TEXT("Base ApplyAction"));
}
