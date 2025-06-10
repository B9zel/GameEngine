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

			PrimitiveComponent();

		public:

			virtual PrimitiveProxy* GetSceneProxy() const;
			virtual PrimitiveProxy* GetUpdateProxy() const;

			//Test
			PrimitiveProxy* sceneProxy;

		};
	}
}