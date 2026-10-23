#include <Runtime/CoreObject/Include/Object.h>
#include <Runtime/CoreObject/Include/ObjectGlobal.h>
#include <Core/includes/Memory/SerializeArchive.h>
#include <Core/includes/World.h>
#include <Core/includes/Memory/SaveManager.h>
#include <Core/includes/Engine.h>

Object::Object(const CoreEngine::InitializeObject& Initialize)
{
	m_World = nullptr;
	m_Outer = nullptr;
	SetFlag(StateObjectFlagGC, static_cast<uint32>(ObjectGCFlags::LiveObject));
	PrivateClass = Initialize.Class;
	CHECK(PrivateClass);

	ObjectID.GenerateID();
	Name = PrivateClass->Name;
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
CoreEngine::Reflection::ClassField* Object::GetClass() const
{
	return PrivateClass;
}

const CoreEngine::UUID& Object::GetUUID() const
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

bool Object::GetHasDeserialized() const
{
	return HasDeserialize;
}

void Object::PreEditChangeProperty(CoreEngine::Reflection::PropertyField& Property)
{
}

void Object::PostEditChangeProperty(CoreEngine::Reflection::PropertyField& Property)
{
}

void Object::PreSerialize()
{
	HasSerialize = false;
}

void Object::Serialize(CoreEngine::SerializeAchive& Archive)
{
	if (!Archive.TrySerializeObject(this)) return;
	HasSerialize = true;

	CoreEngine::Reflection::ClassField* Class = GetClass();
	CoreEngine::SerializeAchive::ObjectScope Scope(Archive, GetUUID().GetID());
	Archive.SerializeData("NameClass", GetClass()->Name);

	while (Class != nullptr)
	{
		for (auto* Property : Class->PropertyFileds)
		{
			if (static_cast<uint32>(Property->Params) & static_cast<uint32>(CoreEngine::Reflection::EPropertyFieldParams::Transient)) continue;
			Property->Serialize(Archive, this);
		}

		Class = Class->ParentClass;
	}
	OnSerialize(Archive);
}

void Object::PreDeserialize()
{
	HasDeserialize = false;
}

void Object::OnPreFieldsDeserialize(CoreEngine::SerializeAchive& Data)
{
}

void Object::Deserialize(CoreEngine::SerializeAchive& Data)
{
	if (HasDeserialize) return;
	HasDeserialize = true;

	OnPreFieldsDeserialize(Data);

	CoreEngine::Reflection::ClassField* Class = GetClass();
	CoreEngine::SerializeAchive::ObjectScope Scope(Data, GetUUID().GetID());
	// Data.PushPrefix(GetName());
	// Data.PushPrefix(ToString(GetUUID().GetID()));
	while (Class != nullptr)
	{
		for (auto* Property : Class->PropertyFileds)
		{
			// Data.PushPrefix(Property->Name);
			Property->Deserialize(Data, this);
			// Data.PopPrefix();
		}

		Class = Class->ParentClass;
	}
	OnDeserialize(Data);

	Data.PopPrefix();
}

void Object::SetOuter(Object* Outer)
{
	m_Outer = Outer;
}

Object* Object::GetOuter() const
{
	return m_Outer;
}

void Object::MarkGarbage()
{
	SetFlag(StateObjectFlagGC, static_cast<uint64>(ObjectGCFlags::Garbage));
}

void Object::OnDeserialize(CoreEngine::SerializeAchive& Data)
{
}

void Object::OnSerialize(CoreEngine::SerializeAchive& Archive)
{
}

void Object::SetUUID(const CoreEngine::UUID& newUUID)
{
	ObjectID = newUUID;
}

void Object::ClearUUID()
{
	ObjectID.Release();
}

const String& Object::GetName() const
{
	return Name;
}
