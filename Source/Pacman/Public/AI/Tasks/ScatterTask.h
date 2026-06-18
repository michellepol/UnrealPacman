#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "CoreMinimal.h"

#include "ScatterTask.generated.h"

UCLASS(Blueprintable)
class PACMAN_API UAITask_Scatter : public UBTTaskNode {
  GENERATED_BODY()

public:
  UAITask_Scatter();

protected:
  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp,
                                           uint8 *NodeMemory) override;

  virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent &OwnerComp,
                                         uint8 *NodeMemory) override;
};