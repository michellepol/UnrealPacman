// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "CoreMinimal.h"

#include "PinkChaseTask.generated.h"

///@brief Pink Ghost chase behavior
///
///@details Move Pink Ghost to 4 tiles ahead of Pacman
UCLASS(Blueprintable)
class PACMAN_API UAITask_PinkChase : public UBTTaskNode {
  GENERATED_BODY()

public:
  UAITask_PinkChase();

  const static uint kTilesAheadPacman = 3;

protected:
  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp,
                                          uint8 *NodeMemory) override;

  virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent &OwnerComp,
                                        uint8 *NodeMemory) override;
};
