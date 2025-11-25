#include <Runtime/CoreObject/Include/Object.h>
#include <Runtime/CoreObject/Include/ObjectGlobal.h>
#include <Core/includes/Memory/SerializeArchive.h>
#include <Core/includes/World.h>
#include <Core/includes/Memory/SaveManager.h>


namespace CoreEngine
{
	namespace Runtime
	{
		Object::Object(const InitializeObject& Initialize)
		{
			m_World = nullptr;
			m_Outer = nullptr;
			SetFlag(StateObjectFlagGC, static_cast<uint32>(ObjectGCFlags::LiveObject));
			PrivateClass = Initialize.Class;
			CHECK(PrivateClass);

			ObjectID.GenerateID();
			Name = PrivateClass->Name;

			InitProperties();
		}
		void Object::InitProperties()
		{
			SetWorld(Engine::Get()->GetWorld());
		}
		void Object::SetWorld(World* newWorld)
		{
			m_World = newWorld;
		}
		World* Object::GetWorld()
		{
			return m_World;
		}
		Reflection::ClassField* Object::GetClass() const
		{
			return PrivateClass;
		}

		const UUID& Object::GetUUID() const
		{
			return ObjectID;
		}
		void Object::SetName(const String& NewName)
		{
			Name = NewName;
		}

		uint32 Object::GetGCState() const
		{
			return StateObjectFlagGC;
		}

		bool Object::GetHasSerialized() const
		{
			return HasSerialize;
		}

		void Object::PreSerialize()
		{
			HasSerialize = false;
		}

		void Object::Serialize(SerializeAchive& Archive)
		{
			if (HasSerialize) return;
			HasSerialize = true;


			Reflection::ClassField* Class = GetClass();
			Archive.PushPrefix(GetName());
			Archive.SerializeData("NameClass", GetClass()->Name);
			while (Class != nullptr)
			{
				for (auto* Property : Class->PropertyFileds)
				{
					Property->Serialize(Archive, this);
				}

				Class = Class->ParentClass;
			}
			//GetWorld()->GetSaveManager()->SaveSceneSerializedData();
			Archive.PopPrefix();
		}

		const String& Object::GetName() const
		{
			return Name;
		}
	}
}