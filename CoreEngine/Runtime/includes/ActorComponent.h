#pragma once
#include <Runtime/CoreObject/Include/Object.h>
#include <Core/includes/UpdateFunction.h>
#include <ActorComponent.generated.h>

class ActorComponent;
class Object;

namespace CoreEngine
{
	class UpdateFunction;

	namespace Runtime
	{

		class UpdateActorComponentFunction : public UpdateFunction
		{
		public:

			UpdateActorComponentFunction() : UpdateDelegate(nullptr, nullptr)
			{
				CanUpdate = true;
				Interval = 0.0f;
				LastTimeUpdate = 0.0f;
				stage = EStageUpdate::PRE_UPDATE;
			}

		public:

			virtual void ExecuteUpdate(float deltaTime) override
			{
				if (!CanUpdate) return;

				LastTimeUpdate += deltaTime;
				if (LastTimeUpdate >= Interval)
				{
					UpdateDelegate.Invoke(std::move(deltaTime));
					LastTimeUpdate = 0.0f;
				}
			}

			void SetUpdateMethod(void (ActorComponent::*method)(float), ActorComponent* obj)
			{
				UpdateDelegate = MethodPtr<ActorComponent, void(float)>(obj, method);
			}

		private:

			MethodPtr<ActorComponent, void(float)> UpdateDelegate;
		};

	} // namespace Runtime
} // namespace CoreEngine

RCLASS();
class ActorComponent : public Object
{
	GENERATED_BODY()

public:

	ActorComponent(const CoreEngine::InitializeObject& InitParam);

public:

	virtual void BeginPlay();

	virtual void InitProperties() override;
	virtual void RegisteredComponent();
	virtual void PreRegisterComponent();
	virtual void UpdateComponent(float deltaTime);

	virtual void DestroyComponent();

	void SetOwner(Actor* Owner);
	Actor* GetOwner() const;
	bool GetIsActive() const;
	bool GetIsCreatedNative() const;

protected:

	CoreEngine::Runtime::UpdateActorComponentFunction updateFunc;

	bool isRegistered = false;
	bool isActivate = true;

	Actor* Owner;

private:

	// if true created before initialize Actor, if false created runtime
	bool IsCreatedNative{false};
};
