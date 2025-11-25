#include <Core/includes/Level.h>
#include <Runtime/includes/Actor.h>
#include <Core/includes/World.h>



namespace CoreEngine
{
	Level::Level(const InitializeObject& Object) : Object(Object)
	{
	}
	const DArray<Runtime::Actor*>& Level::GetActors() const
	{
		return m_Actors;
	}

	void Level::ActorInitialize()
	{
		for (auto& Actor : GetActors())
		{
			Actor->InitComponents();
		}

		for (auto& Actor : GetActors())
		{
			Actor->InitProperties();
		}

	}

	void Level::InitProperties()
	{
		
	}

	void Level::PreSerialize()
	{
		Object::PreSerialize();

		for (auto* Object : m_Objects)
		{
			Object->PreSerialize();
		}
		for (auto* Actor : m_Actors)
		{
			Actor->PreSerialize();
		}
	}

	void Level::Serialize(SerializeAchive& Achive)
	{
		Object::Serialize(Achive);

		for (auto* Object : m_Objects)
		{
			if (!Object->GetHasSerialized())
			{
				Object->Serialize(Achive);
			}
		}
		for (auto* Actor : m_Actors)
		{
			if (!Actor->GetHasSerialized())
			{
				Actor->Serialize(Achive);
			}
		}
	}

	void Level::AddActor(Runtime::Actor* newActor)
	{
		m_Actors.push_back(newActor);
		m_ActorsGC.push_back(newActor);
	}
}
