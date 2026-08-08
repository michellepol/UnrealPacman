// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "CoreMinimal.h"

#include "PinkChaseTask.generated.h"

class ATile;

///@brief Pink Ghost chase behavior
///
///@details Move Pink Ghost to 4 tiles ahead of Pacman
UCLASS(Blueprintable)
class PACMAN_API UAITask_PinkChase : public UBTTaskNode {
  GENERATED_BODY()

public:
  UAITask_PinkChase();

  const static int32 kTilesAheadPacman = 3;

protected:
  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp,
                                          uint8 *NodeMemory) override;

  virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent &OwnerComp,
                                        uint8 *NodeMemory) override;

private:
  ATile *PreviousTile = nullptr;

  ///@brief Check if a tile is behind the ghost
  ///
  ///@param Tile The tile to check
  ///@param OwnerComp The behavior tree component to get the ghost controller
  ///@return true if the tile is behind the ghost, false otherwise
  bool IsBehind(ATile *Tile, UBehaviorTreeComponent &OwnerComp) const;
};
