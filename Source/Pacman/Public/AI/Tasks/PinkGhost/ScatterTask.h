// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "CoreMinimal.h"

#include "ScatterTask.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPinkScatterTask, Log, All);

///@brief Pink Ghost scatter behavior
///
///@details Move Pink Ghost to left upper corner of map and set patrolling mode
UCLASS(Blueprintable)
class PACMAN_API UAITask_PinkScatter : public UBTTaskNode {
  GENERATED_BODY()

public:
  UAITask_PinkScatter();

protected:
  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp,
                                          uint8 *NodeMemory) override;

  virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent &OwnerComp,
                                        uint8 *NodeMemory) override;
};
