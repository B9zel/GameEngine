#pragma once
#include <Runtime/includes/SceneComponent.h>



namespace CoreEngine
{
	class PrimitiveProxy;

	namespace Runtime
	{
		class PrimitiveComponent : public SceneComponent
		{

		public:

			PrimitiveProxy* sceneProxy;
		};
	}
}