#include <Render/includes/Scene/Scene.h>
#include <Core/includes/Level.h>
#include <Runtime/includes/Actor.h>
#include <Runtime/includes/PrimitiveComponent.h>
#include <Render/includes/Render.h>
#include <Core/includes/PrimitiveProxy.h>
#include <Core/includes/LightProxy.h>
#include <Runtime/includes/BaseLightComponent.h>.h>


namespace CoreEngine
{

	namespace Render
	{
		void Scene::CollectProxy()
		{
			if (!GetWorld()) return;
			m_RenderProxy.clear();
			m_LightProxy.clear();


			for (auto& Level : GetWorld()->GetLevels())
			{
				for (auto& Actor : Level->GetActors())
				{
					for (auto* Component : Actor->GetComponents())
					{
						if (auto* Primitive = dynamic_cast<Runtime::PrimitiveComponent*>(Component))
						{
							m_RenderProxy.emplace_back(Primitive->GetUpdateProxy());
						}
						else if (auto* Light = dynamic_cast<Runtime::BaseLightComponent*>(Component))
						{
							m_LightProxy.emplace_back(Light->GetLightProxy());
						}
					}
				}
			}

		}

		void Scene::SetWorld(World* world)
		{
			m_World = world;
		}

		World* Scene::GetWorld() const
		{
			return m_World;
		}

		void Scene::StartRender()
		{
			if (!m_RenderProxy.empty())
			{
				Engine::Get()->GetRender()->RenderPipelineProxy(m_RenderProxy, m_LightProxy);
			}
		}
	}
}


