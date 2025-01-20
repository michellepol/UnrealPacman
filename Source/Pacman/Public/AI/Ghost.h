// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#include "PacmanCharacter.h"

#include "Ghost.generated.h"

class ANavModifierVolume;
class UNavArea;

UENUM(BlueprintType)
enum class EGhostType : uint8 {
  kPink UMETA(DisplayName = "Pink"),
  kRed UMETA(DisplayName = "Red"),
  kBlue UMETA(DisplayName = "Blue"),
  kYellow UMETA(DisplayName = "Yellow")
};

UCLASS()
class PACMAN_API AGhost : public APacmanCharacter {
  GENERATED_BODY()

public:
  AGhost();

  EGhostType GetGhostType();

protected:
  virtual void BeginPlay() override;

public:
  virtual void Tick(float DeltaTime) override;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  TSubclassOf<ANavModifierVolume> NavModifierVolumeClass;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  TSubclassOf<UNavArea> NavAreaClass;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  EGhostType GhostType;

private:
  ANavModifierVolume *NavModifierVolume = nullptr;
};
