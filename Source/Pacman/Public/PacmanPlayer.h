// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "PacmanPlayer.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class PACMAN_API APacmanPlayer : public ACharacter {
  GENERATED_BODY()

public:
  APacmanPlayer();

  UFUNCTION()
  void MoveY(const FInputActionInstance &instance);

  UFUNCTION()
  void MoveX(const FInputActionInstance &instance);

  // Forward and backwards
  UPROPERTY(EditDefaultsOnly, Category = Input)
  UInputAction *MoveYAction;

  // Left and right
  UPROPERTY(EditDefaultsOnly, Category = Input)
  UInputAction *MoveXAction;

protected:
  virtual void BeginPlay() override;

public:
  virtual void Tick(float DeltaTime) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(
      class UInputComponent *PlayerInputComponent) override;

private:
  UPROPERTY(EditAnywhere, Category = Input)
  UInputMappingContext *InputMappingContext;

  UPROPERTY(EditAnywhere, Category = Input)
  float Speed = 200.0f;

  UPROPERTY(EditAnywhere, Category = Input)
  float Sign = 1;

  UPROPERTY(EditAnywhere, Category = Input)
  FVector WorldDirection = GetActorForwardVector();

  bool CheckIntention() const;
};
