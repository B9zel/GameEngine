#include <Core/includes/UpdateFunction.h>
#include <Runtime/CoreObject/Include/Object.h>




namespace CoreEngine
{

	EStageUpdate UpdateFunction::GetStage() const
	{
		return stage;
	}
}