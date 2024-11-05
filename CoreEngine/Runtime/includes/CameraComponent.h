#pragma once
#include <Runtime/includes/SceneComponent.h>
#include <Math/includes/Matrix.h>


namespace CoreEngine
{
	namespace Runtime
	{
		class CameraComponent : public SceneComponent
		{


		protected:

			FMatrix4x4 viewMatrix;
		};
	}
}