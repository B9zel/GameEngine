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
		class Framebuffer;

		class Render
		{
		public:

			Render();

		public:

			static UniquePtr<Render> Create();

			virtual void ClearBuffersScreen() = 0;
			virtual void SetViewProjectionMatrix(const FMatrix4x4& View, const FMatrix4x4& Projection) = 0;
			virtual void RenderPipelineProxy(const DArray<PrimitiveProxy*>& Primitives, const DArray<LightProxy*>& Lights);
			virtual Framebuffer* GetRenderSceneBuffer() const = 0;
			virtual void SetResolutionScale(const FVector2 Resolition) = 0;

		protected:

			virtual void RenderStaticMeshProxy(const StaticMeshProxy* Proxy, const DArray<LightProxy*>& Lights, const DArray<FMatrix4x4>& LightDirecion) = 0;
			// Test mthod
			virtual void RenderProxy(const PrimitiveProxy* Proxy) = 0;

		private:

			UniquePtr<RendererAPI> m_renderAPI;

			static bool isInit;
		};
	}
}