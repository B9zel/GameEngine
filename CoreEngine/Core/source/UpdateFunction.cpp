#include <Core/includes/UpdateFunction.h>
#include <Runtime/CoreObject/Include/Object.h>




namespace CoreEngine
{
	UpdateFunction::UpdateFunction() : UpdateDelegate(nullptr, nullptr)
	{
		CanUpdate = true;
		Inverval = 0.0f;
		LastTimeUpdate = 0.0f;
		stage = EStageUpdate::PRE_UPDATE;
	}

	void UpdateFunction::ExecuteUpdate(float deltaTime)
	{
		if (!CanUpdate) return;

		LastTimeUpdate += deltaTime;
		if (LastTimeUpdate >= Inverval)
		{
			UpdateDelegate.Invoke(std::move(deltaTime));
			LastTimeUpdate = 0.0f;
		}
	}

	template<class FuncClass>
	void UpdateFunction::SetUpdateMethod(Runtime::Object* obj, void(FuncClass::* method)(float))
	{
		UpdateDelegate = MethodPtr<Runtime::Object, void(float)>(obj, method);
	}
	EStageUpdate UpdateFunction::GetStage() const
	{
		return stage;
	}
}