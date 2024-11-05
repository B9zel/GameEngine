#pragma once
#include <Runtime/includes/Actor.h>


namespace CoreEngine
{
	namespace Runtime
	{
		class CameraComponent;

		class CameraManager : public Actor
		{
		public:


			virtual void Update(float deltaTime) override;
			virtual void CameraUpdate(float deltaTime);


		private:

			float FieldOfView;
		};
	}
}