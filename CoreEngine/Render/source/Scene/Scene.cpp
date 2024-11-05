#include <Render/includes/Scene/Scene.h>
#include <Core/includes/Level.h>


namespace CoreEngine
{
	namespace Render
	{
		void Scene::CollectProxy()
		{
			if (!GetWorld()) return;

			for (auto& Level : GetWorld()->GetLevels())
			{
				for (auto& Actor : Level->GetActors())
				{
					//m_RenderProxy.emplace_back(Actor)
				}
			}

		}

		void Scene::SetWorld(World* world)
		{
			m_World = world;
		}

		World* Scene::GetWorld()
		{
			return m_World;
		}
	}
}


