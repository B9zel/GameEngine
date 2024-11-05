#include <Runtime/includes/CameraManager.h>

#include <Runtime/includes/CameraComponent.h>


namespace CoreEngine
{
	namespace Runtime
	{
		void CameraManager::Update(float deltaTime)
		{
			Actor::Update(deltaTime);

			CameraUpdate(deltaTime);
		}

		void CameraManager::CameraUpdate(float deltaTime)
		{
			
		}
	}
}

