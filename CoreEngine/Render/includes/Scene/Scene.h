#pragma once
#include <Render/includes/Scene/SceneInterface.h>
#include <Core/includes/PrimitiveProxy.h>
#include <Core/includes/Base.h>
#include <Core/includes/World.h>



namespace CoreEngine
{
	namespace Render
	{

		class SceneInterface;

		class Scene : public SceneInterface
		{
		public:

			Scene() = default;

		public:

			virtual void CollectProxy() override;

			virtual void SetWorld(World* world) override;
			World* GetWorld() const;

		protected:

			void StartRender();

		private:

			DArray<PrimitiveProxy*> m_RenderProxy;
			World* m_World;
		};
	}
}