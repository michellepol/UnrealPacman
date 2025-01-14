#include "AI/Tasks/FrightenedTask.h"

#include "AIController.h"

UAITask_Frigthened::UAITask_Frigthened() { NodeName = "Ghost Frightened"; }

EBTNodeResult::Type
UAITask_Frigthened::ExecuteTask(UBehaviorTreeComponent &OwnerComp,
                                uint8 *NodeMemory) {

  AAIController *AIController = OwnerComp.GetAIOwner();

  APawn *ControlledPawn = Cast<APawn>(AIController->GetOwner());

  return EBTNodeResult::Type::Succeeded;
}

EBTNodeResult::Type
UAITask_Frigthened::AbortTask(UBehaviorTreeComponent &OwnerComp,
                              uint8 *NodeMemory) {
  Super::AbortTask(OwnerComp, NodeMemory);

  return EBTNodeResult::Type::Aborted;
}
