#include <Core/includes/Level.h>
#include <algorithm>

#include <Core/includes/World.h>
#include <Core/includes/Engine.h>

#include <Core/includes/Memory/SerializeArchive.h>

Level::Level(const CoreEngine::InitializeObject& Object) : Object(Object)
{
}
const DArray<Actor*>& Level::GetActors() const
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

void Level::OnSerialize(CoreEngine::SerializeAchive& Achive)
{
	Object::OnSerialize(Achive);
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

void Level::OnDeserialize(CoreEngine::SerializeAchive& Data)
{
	Object::OnDeserialize(Data);
	using namespace CoreEngine::Reflection;

	Deinitialize();

	bool Success = false;
	const auto& Objects = Data.Data()["Objects"];
	const auto& MapRegister = CoreEngine::Reflection::MapRegistryClass::Instance();
	for (auto It = Objects.begin(); It != Objects.end(); ++It)
	{
		const uint64 savedUUID = std::stoull(It.key());
		auto Elem = It.value();

		ClassField* Class = MapRegister.GetClassField(Elem["NameClass"].get<String>());
		Actor* NewActor = CreateObjectFromClass(Class);

		if (!NewActor) continue;

		NewActor->SetUUID(savedUUID);
		const auto& SerializedComponetns = Elem["Components"];

		for (auto ComponentIt = SerializedComponetns.begin(); ComponentIt != SerializedComponetns.end(); ++ComponentIt)
		{
			auto Ref = ComponentIt.value();
			const uint64 ID = Ref["$ref"].get<uint64>();

			auto ComponentData = Objects[ToString(ID)];
			const String Name = ComponentData["Name"];
			const String NameClass = ComponentData["NameClass"];

			for (ActorComponent* Component : NewActor->GetComponents_Source())
			{
				if (Component->GetName() == Name && Component->GetClass()->IsChildClassOf(MapRegister.GetClassField(NameClass)))
				{
					Component->SetUUID(ID);
					break;
				}
			}
		}

		/*	for (auto& component : NewActor->GetComponents())
			{
				component->SetUUID(CoreEngine::UUID(std::stoull(Elem["Components"][std::to_string(component->GetUUID().GetID())]["UUID"].get<String>())));
			}*/

		EG_LOG(CoreEngine::CORE, ELevelLog::INFO, Elem.dump(4));
	}

	/*for (auto* Object : Engine::Get()->GetMemoryManager()->GetGarbageCollector()->GetAllObjects())
	{
		if (!Object->GetHasDeserialized())
		{
			Object->Deserialize(Data);
		}
	}*/

	for (auto* Actor : m_Actors)
	{
		if (!Actor->GetHasDeserialized())
		{
			Actor->Deserialize(Data);
		}
	}
}

void Level::Deinitialize()
{
	const auto actors = m_Actors;
	for (auto* actor : actors)
	{
		if (actor)
		{
			actor->Destroy();
		}
	}

	m_Actors.clear();
}

void Level::RouteEndPlayRemoveFromWorld()
{
	for (auto* actor : m_Actors)
	{
		actor->RouteEndPlay();
	}
}

void Level::AddActor(Actor* newActor)
{
	CHECK(newActor);
	CHECK(newActor->m_Level == nullptr);
	newActor->m_Level = this;
	m_Actors.push_back(newActor);
}

void Level::RemoveActor(Actor* actor)
{
	const auto actorIt = std::find(m_Actors.begin(), m_Actors.end(), actor);
	if (actorIt != m_Actors.end())
	{
		m_Actors.erase(actorIt);
		actor->m_Level = nullptr;
	}
}

Actor* Level::CreateObjectFromClass(CoreEngine::Reflection::ClassField* Class)
{
	if (Class->IsChildClassOf(Actor::GetStaticClass()))
	{
		return GetWorld()->SpawnActor<Actor>(Class, nullptr, FTransform());
	}

	return nullptr;
}
