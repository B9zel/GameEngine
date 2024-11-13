#pragma once
#include <Runtime/CoreObject/Include/Object.h>
#include <Core/includes/UpdateFunction.h>



namespace CoreEngine
{
	class UpdateFunction;

	namespace Runtime
	{
		class ActorComponent;
		class Object;


		class UpdateActorComponentFunction : public UpdateFunction
		{
		public:

			UpdateActorComponentFunction() : UpdateDelegate(nullptr, nullptr)
			{
				CanUpdate = true;
				Inverval = 0.0f;
				LastTimeUpdate = 0.0f;
				stage = EStageUpdate::PRE_UPDATE;
			}

		public:

			virtual void ExecuteUpdate(float deltaTime) override
			{
				if (!CanUpdate) return;

				LastTimeUpdate += deltaTime;
				if (LastTimeUpdate >= Inverval)
				{
					UpdateDelegate.Invoke(std::move(deltaTime));
					LastTimeUpdate = 0.0f;
				}
			}
			
			void SetUpdateMethod(void(ActorComponent::* method)(float), ActorComponent* obj)
			{
				UpdateDelegate = MethodPtr<Runtime::ActorComponent, void(float)>(obj, method);
			}


		private:

			MethodPtr<Runtime::ActorComponent, void(float)> UpdateDelegate;
		};


		class ActorComponent : public CoreEngine::Runtime::Object
		{
		public:

			virtual void InitProperties() override;
			virtual void RegisteredComponent();
			virtual void TickComponent(float deltaTime) = 0;
			bool GetIsActive() const;

		protected:

			UpdateActorComponentFunction updateFunc;

			bool isRegistered = false;
			bool isActivate = true;
		};
	}
}