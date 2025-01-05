// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Ghost.generated.h"

UCLASS()
class PACMAN_API AGhost : public ACharacter {
  GENERATED_BODY()

public:
  AGhost();

protected:
  virtual void BeginPlay() override;

public:
  virtual void Tick(float DeltaTime) override;
};
