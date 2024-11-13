#pragma once
#include <Core/includes/Base.h>
#include <Runtime/CoreObject/Include/Object.h>
#include <Core/includes/UpdateFunction.h>
#include <Core/includes/World.h>
#include <Core/includes/UpdateManager.h>


namespace CoreEngine
{
	class UpdateFunction;

	namespace Runtime
	{
		class SceneComponent;
		class ActorComponent;
		class Controller;


		class UpdateActorFunction : public UpdateFunction
		{
		public:

			UpdateActorFunction() : UpdateDelegate(nullptr, nullptr)
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
			void SetUpdateMethod(void(Actor::* method)(float), Actor* obj)
			{
				UpdateDelegate = MethodPtr<Runtime::Actor, void(float)>(obj, method);
			}


		private:

			MethodPtr<Runtime::Actor, void(float)> UpdateDelegate;
		};



		class Actor : public Object
		{
		public:

			friend UpdateFunction;

		public:

			Actor();

		public:

			virtual void InitProperties() override;
			virtual void DispatchBeginPlay();
			virtual void Registered();
			virtual void InitComponents();


			template<class ReturnType>
			ReturnType* CreateSubObject();

			SceneComponent* GetRootComponent() const;
			void SetRootComponent(SceneComponent* root);

			FVector GetActorLocation() const;
			FVector GetActorScale() const;
			FVector GetActorRotation() const;
			Actor* GetOwner() const;

			void SetActorLocation(const FVector& newLocation);
			void SetActorSclae(const FVector& newScale);
			void SetActorRotation(const FVector& newRotation);
			void SetOwner(Actor* newOwner);

			const DArray<ActorComponent*>& GetComponents() const;

		protected:

			virtual void BeginPlay();
			virtual void Update(float deltaTime);


		private:
			
			void NativeUpdate(float deltaTime);

		protected:

			SceneComponent* RootComponent;
			UpdateActorFunction actorUpdate;

			Actor* Owner;

			DArray<ActorComponent*> Components;
			DArray<GB::GBNotify<ActorComponent*>> ComponentsGC;

		private:

			bool isBeginedPlay;
			bool isRegister;
		};
		template<class ReturnType>
		inline ReturnType* Actor::CreateSubObject()
		{
			ReturnType* obj = CreateObject<ReturnType>(this);
			Components.emplace_back(obj);
			ComponentsGC.emplace_back(obj);
			return obj;
			
		}
	}
}