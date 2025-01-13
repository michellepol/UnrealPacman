// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "CoreMinimal.h"

#include "FrightenedTask.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFrigthenedTask, Log, All);

///@brief  Ghost frightened behavior
UCLASS(Blueprintable)
class PACMAN_API UAITask_Frigthened : public UBTTaskNode {
  GENERATED_BODY()

public:
  UAITask_Frigthened();

protected:
  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp,
                                          uint8 *NodeMemory) override;

  virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent &OwnerComp,
                                        uint8 *NodeMemory) override;
};
