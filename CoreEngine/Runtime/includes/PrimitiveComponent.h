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

			PrimitiveProxy* GetSceneProxy() const;

		protected:

			PrimitiveProxy* sceneProxy;
		};
	}
}