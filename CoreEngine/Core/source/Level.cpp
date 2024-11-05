#include <Core/includes/Level.h>
#include <Runtime/CoreObject/Include/Object.h>
#include <Runtime/includes/Actor.h>


namespace CoreEngine
{	
	const DArray<Runtime::Actor*>& Level::GetActors() const
	{
		return m_Actors;
	}
}
