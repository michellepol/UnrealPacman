// Fill out your copyright notice in the Description page of Project Settings.

#include "PacmanPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputTriggers.h"

// Sets default values
APacmanPlayer::APacmanPlayer() {
  // Set this character to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APacmanPlayer::BeginPlay() { Super::BeginPlay(); }

// Called to bind functionality to input
void APacmanPlayer::SetupPlayerInputComponent(
    UInputComponent *PlayerInputComponent) {

  APlayerController *PlayerController =
      Cast<APlayerController>(GetController());

  if (InputMappingContext && PlayerController) {
    UEnhancedInputLocalPlayerSubsystem *Subsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
            PlayerController->GetLocalPlayer());

    Subsystem->ClearAllMappings();
    Subsystem->AddMappingContext(InputMappingContext, 0);
  }

  UEnhancedInputComponent *Input =
      Cast<UEnhancedInputComponent>(PlayerInputComponent);

  if (!Input) {
    UE_LOG(LogTemp, Display, TEXT("No enhanced input component"));
  }

  if (MoveYAction) {
    Input->BindAction(MoveYAction, ETriggerEvent::Triggered, this,
                      &APacmanPlayer::MoveY);
  }

  if (MoveXAction) {
    Input->BindAction(MoveXAction, ETriggerEvent::Triggered, this,
                      &APacmanPlayer::MoveX);
  }
}

void APacmanPlayer::MoveX(const FInputActionInstance &Instance) {
  float Direction = Instance.GetValue().Get<float>();

  Sign = -Direction;
  WorldDirection = GetActorRightVector();
}

void APacmanPlayer::MoveY(const FInputActionInstance &Instance) {
  float Direction = Instance.GetValue().Get<float>();

  Sign = Direction;
  WorldDirection = GetActorForwardVector();
}

void APacmanPlayer::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  if (CheckIntention()) {
    AddMovementInput(WorldDirection, Sign * Speed);
  }
}

bool APacmanPlayer::CheckIntention() const { return true; }
