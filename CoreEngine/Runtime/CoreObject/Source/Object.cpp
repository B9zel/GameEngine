#include <Runtime/CoreObject/Include/Object.h>
#include <Runtime/CoreObject/Include/ObjectGlobal.h>


namespace CoreEngine
{
	namespace Runtime
	{
		Object::Object(const InitializeObject& Initialize)
		{
			m_World = nullptr;
			m_Outer = nullptr;
			SetFlag(StateObjectFlagGC, static_cast<uint32>(ObjectGCFlags::LiveObject));
			IsMarkedGC = false;
			PrivateClass = Initialize.Class;
			ObjectID.GenerateID();

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

		bool Object::GetIsMarked() const
		{
			return IsMarkedGC;
		}

		void Object::SetMarked(const bool Value)
		{
			IsMarkedGC = Value;
		}
		uint32 Object::GetGCState() const
		{
			return StateObjectFlagGC;
		}
	}
}