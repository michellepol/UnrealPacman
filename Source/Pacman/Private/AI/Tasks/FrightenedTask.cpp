#include "AI/Tasks/FrightenedTask.h"

#include "AIController.h"

#include "AI/GhostController.h"

UAITask_Frigthened::UAITask_Frigthened() { NodeName = "Ghost Frightened"; }

EBTNodeResult::Type
UAITask_Frigthened::ExecuteTask(UBehaviorTreeComponent &OwnerComp,
                                uint8 *NodeMemory) {

  AGhostController *GhostController =
      Cast<AGhostController>(OwnerComp.GetAIOwner());
  if (!GhostController || !GhostController->GetPawn()) {
    return EBTNodeResult::Failed;
  }

  GhostController->FrigthenedMove();

  return EBTNodeResult::Type::Succeeded;
}

EBTNodeResult::Type
UAITask_Frigthened::AbortTask(UBehaviorTreeComponent &OwnerComp,
                              uint8 *NodeMemory) {
  Super::AbortTask(OwnerComp, NodeMemory);

  return EBTNodeResult::Type::Aborted;
}
