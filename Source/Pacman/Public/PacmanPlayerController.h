// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PacmanPlayerController.generated.h"

UCLASS()
class PACMAN_API APacmanPlayerController : public APlayerController {
  GENERATED_BODY()

public:
  void SetupInputComponent() override;

private:
};
