#pragma once
#include <Runtime/CoreObject/Include/Object.h>
#include <Core/includes/UpdateFunction.h>



namespace CoreEngine
{
	class UpdateFunction;

	namespace Runtime
	{
		class Object;
		class ActorComponent : public CoreEngine::Runtime::Object
		{
		public:

			virtual void InitProperties() override;
			virtual void RegisteredComponent();
			virtual void TickComponent(float deltaTime) = 0;

		protected:

			UpdateFunction updateFunc;

			bool isRegistered;
		};
	}
}