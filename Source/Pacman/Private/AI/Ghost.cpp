// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Ghost.h"

AGhost::AGhost() { PrimaryActorTick.bCanEverTick = true; }

void AGhost::BeginPlay() { Super::BeginPlay(); }

void AGhost::Tick(float DeltaTime) { Super::Tick(DeltaTime); }
