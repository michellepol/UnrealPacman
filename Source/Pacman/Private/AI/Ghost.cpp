// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Ghost.h"

#include "Components/SkeletalMeshComponent.h"

// Sets default values
AGhost::AGhost() {
  // Set this pawn to call Tick() every frame.  You can turn this off to improve
  // performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGhost::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void AGhost::Tick(float DeltaTime) { Super::Tick(DeltaTime); }
