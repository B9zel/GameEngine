#pragma once
#include <Core/includes/Base.h>
#include <Core/includes/UpdateManager.h>



namespace CoreEngine
{
	namespace Runtime
	{
		class Object;
	}

	enum class EStageUpdate : uint8;


	// Stores pointer on Update method and decides when to call mthod
	class UpdateFunction
	{
	public:

		UpdateFunction();

		UpdateFunction(const UpdateFunction&) = default;
		UpdateFunction(UpdateFunction&&) = default;

	public:
		
		/*
		* Call bind update function, if LastTimeUpdate more then Interval
		* @param time after last frame 
		*/
		void ExecuteUpdate(float deltaTime);
		template<class FuncClass>
		void SetUpdateMethod(Runtime::Object* obj, void(FuncClass::*method)(float));
		EStageUpdate GetStage() const;

	protected:

		bool CanUpdate;
		float Inverval;
		float LastTimeUpdate;
		EStageUpdate stage;

		MethodPtr<Runtime::Object, void(float)> UpdateDelegate;
	};

}