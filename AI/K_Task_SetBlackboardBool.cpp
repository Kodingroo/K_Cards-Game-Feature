// Copyright Isaac Hayward

#include "K_Task_SetBlackboardBool.h"
#include "BehaviorTree/BlackboardComponent.h"

UK_Task_SetBlackboardBool::UK_Task_SetBlackboardBool()
{
	NodeName = "Set Blackboard Bool";
	bValueToSet = true; // Default to setting the value to true
}

EBTNodeResult::Type UK_Task_SetBlackboardBool::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(BlackboardKey.SelectedKeyName, bValueToSet);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
