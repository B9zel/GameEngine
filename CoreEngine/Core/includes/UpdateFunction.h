#pragma once
#include <Core/includes/Base.h>
#include <Core/includes/UpdateManager.h>



namespace CoreEngine
{
	namespace Runtime
	{
		class Object;
		class Actor;
	}

	enum class EStageUpdate : uint8;


	// Stores pointer on Update method and decides when to call method
	class UpdateFunction
	{
	public:

		friend Runtime::Actor;

	public:

		UpdateFunction() = default;

		UpdateFunction(const UpdateFunction&) = default;
		UpdateFunction(UpdateFunction&&) = default;

	public:
		
		/*
		* Call bind update function, if LastTimeUpdate more then Interval
		* @param time after last frame 
		*/
		virtual void ExecuteUpdate(float deltaTime) = 0;

		EStageUpdate GetStage() const;

	protected:

		bool CanUpdate;
		float Interval;
		float LastTimeUpdate;
		EStageUpdate stage;
	};

}