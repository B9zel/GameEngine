#include <Runtime/includes/GameInstance.h>


namespace CoreEngine
{
	namespace Runtime
	{
		void GameInstance::Update(float deltaTime)
		{
			m_timerManager.Update(deltaTime);
		}
	}
}