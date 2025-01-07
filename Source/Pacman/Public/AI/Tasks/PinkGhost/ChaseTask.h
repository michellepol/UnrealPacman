// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "CoreMinimal.h"

#include "ChaseTask.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTask, Log, All);

UCLASS(Blueprintable)
class PACMAN_API UPinkGhostChase_Task : public UBTTaskNode {
  GENERATED_BODY()

public:
  UPinkGhostChase_Task();

protected:
  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp,
                                          uint8 *NodeMemory) override;

  virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent &OwnerComp,
                                        uint8 *NodeMemory) override;
};
