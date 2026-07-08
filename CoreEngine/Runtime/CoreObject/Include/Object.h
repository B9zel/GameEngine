#pragma once

#include <Core/includes/Base.h>
//  #include <Runtime/CoreObject/Include/ObjectGlobal.h>
#include <Core/includes/UUID.h>
// #include <ReflectionSystem/Include/MetaClass.h>
#include <ReflectionSystem/Include/ReflectionMacros.h>
// #include <ReflectionSystem/Include/RegistryMap/MapRegistryClass.h>
#include <Object.generated.h>

class Engine;

namespace CoreEngine
{
	class Layer;
	template <class T> class ObjectPtr;
	namespace GB
	{
		class GarbageCollector;
	}
} // namespace CoreEngine

enum class ObjectGCFlags : uint64
{
	None = 0,
	RootObject = FLAG_OFFSET(0),
	LiveObject = FLAG_OFFSET(1),
	Unreachable = FLAG_OFFSET(2),
	Garbage = FLAG_OFFSET(3)
};

class World;

namespace CoreEngine
{
	class SerializeAchive;
	template <typename T> class ObjectPtr;

	struct InitializeObject
	{
		Reflection::ClassField* Class = nullptr;
	};

	namespace Runtime
	{

	} // namespace Runtime
} // namespace CoreEngine

RCLASS();
class Object
{

	GENERATED_BODY();

public:

	Object(const CoreEngine::InitializeObject& Initilize);
	virtual ~Object()
	{
		EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "Destroy object");
	}

public:

	virtual void InitProperties();
	virtual CoreEngine::Reflection::ClassField* GetClass() const;

	template <class ReturnType> ReturnType* CreateSubObject(const String& Name);

	void SetWorld(World* newWorld);
	World* GetWorld();

	const CoreEngine::UUID& GetUUID() const;
	const String& GetName() const;

	virtual void SetName(const String& NewName);

	uint32 GetGCState() const;
	bool GetHasSerialized() const;
	bool GetHasDeserialized() const;

	virtual void PreEditChangeProperty(CoreEngine::Reflection::PropertyField& Property);
	virtual void PostEditChangeProperty(CoreEngine::Reflection::PropertyField& Property);

	virtual void StartDestroy()
	{
	}
	virtual void FinishDestroy()
	{
	}

	virtual void PreSerialize();
	void Serialize(CoreEngine::SerializeAchive& Archive);
	virtual void PreDeserialize();
	void Deserialize(CoreEngine::SerializeAchive& Data);

	void SetOuter(Object* Outer);
	Object* GetOuter() const;

	virtual void MarkGarbage();

protected:

	virtual void OnDeserialize(CoreEngine::SerializeAchive& Data);
	virtual void OnSerialize(CoreEngine::SerializeAchive& Archive);

private:

	RPROPERTY();
	Object* m_Outer;

	World* m_World;

	CoreEngine::UUID ObjectID;
	CoreEngine::Reflection::ClassField* PrivateClass;
	RPROPERTY();
	String Name;

	// GC
	uint64 StateObjectFlagGC{0};
	//

	// Serialize
	bool HasSerialize{false};
	// Deserialize
	bool HasDeserialize{false};

	friend CoreEngine::GB::GarbageCollector;
};

template <class ReturnType> inline ReturnType* Object::CreateSubObject(const String& Name)
{
	ReturnType* obj = CreateObject<ReturnType>(this);
	obj->Name = Name;
	return obj;
}
