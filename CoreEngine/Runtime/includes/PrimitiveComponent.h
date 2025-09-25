#pragma once
#include <Runtime/includes/SceneComponent.h>
#include <PrimitiveComponent.generated.h>


namespace CoreEngine
{
	class PrimitiveProxy;

	namespace Runtime
	{
		RCLASS()
			class PrimitiveComponent : public SceneComponent
		{
			GENERATED_BODY()

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