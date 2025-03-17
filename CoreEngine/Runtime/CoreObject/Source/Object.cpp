#include <Runtime/CoreObject/Include/Object.h>
#include <Runtime/CoreObject/Include/ObjectGlobal.h>


namespace CoreEngine
{
	namespace Runtime
	{
		Object::Object()
		{
			m_World = nullptr;
			m_Outer = nullptr;
			InitProperties();
		}
		void Object::InitProperties()
		{
			SetWorld(Engine::Get()->GetWorld().get());
		}
		void Object::SetWorld(World* newWorld)
		{
			m_World = newWorld;
		}
		World* Object::GetWorld()
		{
			return m_World;
		}
	}
}