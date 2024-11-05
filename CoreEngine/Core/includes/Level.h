#pragma once
#include <Core/includes/Base.h>


namespace CoreEngine
{
	namespace Runtime
	{
		class Object;
		class Actor;
	}

	class Level
	{
	public:

		const DArray<Runtime::Actor*>& GetActors() const;
		

	protected:

		DArray<Runtime::Object*> m_Objects;
		DArray<Runtime::Actor*> m_Actors;
	};
}