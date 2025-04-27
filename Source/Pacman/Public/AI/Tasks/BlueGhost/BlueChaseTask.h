// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "CoreMinimal.h"

#include "BlueChaseTask.generated.h"

///@brief Blue Ghost chase behavior
///
///@details Algorithm of task:
/// 1) Calculate position of 2 tiles ahead of pacman
/// 2) Get doubled vector from Red Ghost to 2 tiles ahead of pacman
/// 3) End of doubled vector - target tile for Blue ghost
UCLASS(Blueprintable)
class PACMAN_API UAITask_BlueChase : public UBTTaskNode {
  GENERATED_BODY()

public:
  UAITask_BlueChase();

  const static int kTilesAheadPacman = 3;

protected:
  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp,
                                          uint8 *NodeMemory) override;

  virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent &OwnerComp,
                                        uint8 *NodeMemory) override;
};
