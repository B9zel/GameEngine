#pragma once
#include <Render/includes/RendererAPI.h>

namespace CoreEngine
{
	class PrimitiveProxy;
	class StaticMehsProxy;

	namespace Render
	{
		class Render
		{
		public:

			Render();

			static UniquePtr<Render> Create();
			void RenderPipelineProxy(const DArray<PrimitiveProxy*>& Primitives);

		protected:

			virtual void RenderStaticMeshProxy(const StaticMehsProxy* Proxy) = 0;
			// Test mthod
			virtual void RenderProxy(const PrimitiveProxy* Proxy) = 0;

		private:

			UniquePtr<RendererAPI> m_renderAPI;

			static bool isInit;
		};
	}
}