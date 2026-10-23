#pragma once
#include <Core/includes/ObjectPtr.h>
#include <Runtime/CoreObject/Include/Object.h>
#include <Core/includes/Base.h>
#include <Runtime/includes/Actor.h>
#include "Level.generated.h"

class Object;
class Actor;
class World;

RCLASS()
class Level : public Object
{
	GENERATED_BODY()

private:

	friend World;
	friend Actor;

public:

	Level(const CoreEngine::InitializeObject& Object);

	const DArray<Actor*>& GetActors() const;

	virtual void ActorInitialize();
	virtual void InitProperties() override;

	virtual void PreSerialize() override;
	virtual void OnSerialize(CoreEngine::SerializeAchive& Achive) override;

	virtual void PreDeserialize() override;
	virtual void OnDeserialize(CoreEngine::SerializeAchive& Data) override;

protected:

	virtual void Deinitialize();

private:

	void RouteEndPlayRemoveFromWorld();
	void AddActor(Actor* newActor);
	void RemoveActor(Actor* actor);

	Actor* CreateObjectFromClass(CoreEngine::Reflection::ClassField* Class);

protected:

	RPROPERTY();
	DArray<Object*> m_Objects;

	RPROPERTY();
	DArray<Actor*> m_Actors;
};
