#include <Runtime/CoreObject/Include/Object.h>
#include <Runtime/CoreObject/Include/ObjectGlobal.h>


namespace CoreEngine
{
	namespace Runtime
	{
		
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