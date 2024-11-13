#include <Core/includes/Level.h>
#include <Core/includes/World.h>

#include <Runtime/includes/Actor.h>


namespace CoreEngine
{	
	const DArray<Runtime::Actor*>& Level::GetActors() const
	{
		return m_Actors;
	}

	void Level::ActorInitizize()
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

	void Level::AddActor(Runtime::Actor* newActor)
	{
		m_Actors.emplace_back(newActor);
		m_ActorsGC.push_back(newActor);
	}
}
