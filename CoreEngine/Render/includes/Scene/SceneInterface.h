#pragma once

class World;

namespace CoreEngine
{
	namespace Render
	{
		class SceneInterface
		{
		public:

			virtual void CollectProxy() = 0;
			virtual void StartRender() = 0;
			virtual void SetWorld(World* world) = 0;
		};
	} // namespace Render
} // namespace CoreEngine
