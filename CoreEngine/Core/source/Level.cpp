#include <Core/includes/Level.h>
#include <Runtime/includes/Actor.h>
#include <Core/includes/World.h>
#include <Core/includes/Memory/SerializeArchive.h>


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

	void Level::OnSerialize(SerializeAchive& Achive)
	{
		Object::OnSerialize(Achive);

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

	void Level::PreDeserialize()
	{
		Object::PreDeserialize();

		for (auto* Object : m_Objects)
		{
			Object->PreDeserialize();
		}
		for (auto* Actor : m_Actors)
		{
			Actor->PreDeserialize();
		}
	}

	void Level::OnDeserialize(SerializeAchive& Data)
	{
		Object::OnDeserialize(Data);

		static DArray<Runtime::Actor*> DeleteActors;
		DeleteActors.clear();

		DeleteActors = m_Actors;
		for (auto& i : DeleteActors)
		{
			i->Destroy();
		}

		bool Success = false;
		for (auto& Elem : Data.FindLastNode()["m_Actors"])
		{
			GetWorld()->SpawnActor<Runtime::Actor>(Reflection::MapRegistryClass::Instance().GetClassField(Elem["NameClass"].get<String>()), nullptr);
			EG_LOG(CORE, ELevelLog::INFO, Elem.dump(4));
		}

		for (auto* Object : m_Objects)
		{
			if (!Object->GetHasDeserialized())
			{
				Object->Deserialize(Data);
			}
		}
		Data.PushPrefix("m_Actors");
		for (auto* Actor : m_Actors)
		{
			if (!Actor->GetHasDeserialized())
			{
				Actor->Deserialize(Data);
			}
		}
		Data.PopPrefix();
	}

	void Level::AddActor(Runtime::Actor* newActor)
	{
		m_Actors.push_back(newActor);
		//m_ActorsGC.push_back(newActor);
	}
}
