// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "CoreMinimal.h"

#include "RedChaseTask.generated.h"

///@brief Reg Ghost chase behavior
///
///@details Move Red Ghost to tile of Pacman
UCLASS(Blueprintable)
class PACMAN_API UAITask_RedChase : public UBTTaskNode {
  GENERATED_BODY()

public:
  UAITask_RedChase();

protected:
  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp,
                                          uint8 *NodeMemory) override;

  virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent &OwnerComp,
                                        uint8 *NodeMemory) override;
};
