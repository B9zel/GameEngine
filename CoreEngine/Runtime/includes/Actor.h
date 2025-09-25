#pragma once
#include <Core/includes/Base.h>
#include <Runtime/CoreObject/Include/Object.h>
#include <Core/includes/UpdateFunction.h>
#include <Core/includes/World.h>
#include <Core/includes/UpdateManager.h>
#include <Math/includes/Transform.h>
#include <Actor.generated.h>


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
			void SetUpdateMethod(void(Actor::* method)(float), Actor* obj)
			{
				UpdateDelegate = MethodPtr<Runtime::Actor, void(float)>(obj, method);
			}

		private:

			MethodPtr<Runtime::Actor, void(float)> UpdateDelegate;
		};


		RCLASS();
		class Actor : public Object
		{

			GENERATED_BODY()

		public:

			friend UpdateFunction;

		public:

			Actor();

		public:

			virtual void InitProperties() override;
			virtual void RegisterAll();
			virtual void DispatchBeginPlay();
			virtual void InitComponents();
			virtual void PostSpawnActor();


			void Registered();
			template<class ReturnType>
			ReturnType* CreateSubObject();

			template<class ReturnType>
			ReturnType* CreateRuntimeSubObject();

			SceneComponent* GetRootComponent() const;
			void SetRootComponent(SceneComponent* root);

			FVector GetActorLocation() const;
			FVector GetActorScale() const;
			FVector GetActorRotation() const;
			Transform GetActorTransform() const;
			FVector GetActorForwardVector() const;
			FVector GetActorRightVector() const;
			Actor* GetOwner() const;

			void SetActorLocation(const FVector& newLocation);
			void SetActorScale(const FVector& newScale);
			void SetActorRotation(const FVector& newRotation);
			void SetActorTransform(const Transform& newTransform);
			void SetOwner(Actor* newOwner);

			void AddActorLocation(const FVector& AddValue);
			void AddActorRotation(const FVector& AddValue);

			template<class TClass>
			TClass* FindComponentByClass();
			template<class TClass>
			DArray<TClass*> FindComponentsByClass();

			const DArray<ActorComponent*>& GetComponents() const;

		protected:

			virtual void BeginPlay();
			virtual void Update(float deltaTime);
			virtual void OnRegistered();

		private:

			void NativeUpdate(float deltaTime);

		protected:

			SceneComponent* RootComponent;
			UpdateActorFunction actorUpdate;

			Actor* Owner;

			DArray<ActorComponent*> Components;
			DArray<ObjectPtr<ActorComponent>> ComponentsGC;

		private:

			bool isBeginedPlay;
			bool isRegister;

			FVector m_Direction;
		};
		template<class ReturnType>
		inline ReturnType* Actor::CreateSubObject()
		{
			ReturnType* obj = CreateObject<ReturnType>(this);
			if (obj)
			{
				obj->SetOwner(this);
			}
			if (auto* newClass = dynamic_cast<SceneComponent*>(obj))
			{
				if (RootComponent)
				{
					newClass->SetupToAttachment(RootComponent);
					newClass->SetTransform(RootComponent->GetTransform());
				}


			}
			Components.emplace_back(obj);
			ComponentsGC.emplace_back(obj);
			return obj;

		}
		template<class ReturnType>
		inline ReturnType* Actor::CreateRuntimeSubObject()
		{
			ReturnType* newComponent = CreateSubObject<ReturnType>();
			newComponent->RegistredComponent();

			return newComponent;
		}
		template<class TClass>
		inline TClass* Actor::FindComponentByClass()
		{
			if (!IsParentClass<ActorComponent, TClass>()) return nullptr;
			for (ActorComponent* i : Components)
			{
				if (auto* res = dynamic_cast<TClass*>(i))
				{
					return res;
				}
			}
			return nullptr;
		}
		template<class TClass>
		inline DArray<TClass*> Actor::FindComponentsByClass()
		{
			if (!IsParentClass<ActorComponent, TClass>()) return DArray<TClass*>();
			DArray<TClass*> Res;
			for (ActorComponent* i : Components)
			{
				if (auto* res = dynamic_cast<TClass*>(i))
				{
					Res.emplace_back(res);
				}
			}
			return Res;
		}
	}
}