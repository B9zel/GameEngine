#pragma once
#include <Runtime/CoreObject/Include/Object.h>
#include <Core/includes/TimerManager.h>


namespace CoreEngine
{
	namespace Runtime
	{
		class GameInstance : public Object
		{
		public:

			virtual void Update(float deltaTime);


		private:

			TimerManager m_timerManager;
		};


	}
}