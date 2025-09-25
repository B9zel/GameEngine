#include <Render/includes/Render.h>
#include <Core/includes/PrimitiveProxy.h>
#include <Core/includes/StaticMeshProxy.h>
#include <Core/includes/LightProxy.h>
#include <Platform/Renderer/OpenGL/include/OpenGLRendere.h>


namespace CoreEngine
{

	namespace Render
	{
		bool Render::isInit = false;

		Render::Render()
		{
			CORE_UNASSERT(isInit, "Render already create");
			isInit = true;
		}

		UniquePtr<Render> Render::Create()
		{
			UniquePtr<Render> render;
			switch (RendererAPI::GetAPI())
			{
			case RendererAPI::API::OpenGL:
			{
				render = MakeUniquePtr<OpenGL::OpenGLRender>();
				render->m_renderAPI = RendererAPI::CreateAPI();

				break;
			}
			default:
				break;
			}

			return render;
		}

		void Render::RenderPipelineProxy(const DArray<PrimitiveProxy*>& Primitives, const DArray<LightProxy*>& Lights)
		{
			using namespace CoreEngine::Render;
			for (auto* Primitive : Primitives)
			{
				// Return later !!!!!!!!!!!
				if (StaticMeshProxy* StaticProxy = dynamic_cast<StaticMeshProxy*>(Primitive))
				{
					RenderStaticMeshProxy(StaticProxy, Lights);
				}
				else
				{
					RenderProxy(Primitive);
				}
			}
		}




	}
}