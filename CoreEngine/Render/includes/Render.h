#pragma once
#include <Render/includes/RendererAPI.h>
#include <Math/includes/Matrix.h>


namespace CoreEngine
{
	class PrimitiveProxy;
	class StaticMeshProxy;
	class LightProxy;

	namespace Render
	{
		class Render
		{
		public:

			Render();

		public:

			static UniquePtr<Render> Create();

			virtual void ClearBuffersScreen() = 0;
			virtual void SetViewProjectionMatrix(const FMatrix4x4& View, const FMatrix4x4& Projection) = 0;
			void RenderPipelineProxy(const DArray<PrimitiveProxy*>& Primitives, const DArray<LightProxy*>& Lights);

		protected:

			virtual void RenderStaticMeshProxy(const StaticMeshProxy* Proxy, const DArray<LightProxy*>& Lights) = 0;
			// Test mthod
			virtual void RenderProxy(const PrimitiveProxy* Proxy) = 0;

		private:

			UniquePtr<RendererAPI> m_renderAPI;

			static bool isInit;
		};
	}
}