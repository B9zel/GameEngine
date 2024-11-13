#include <Render/includes/Render.h>
#include <Core/includes/PrimitiveProxy.h>
#include <Core/includes/StaticMeshProxy.h>
#include <Platform/Renderer/OpenGL/include/OpenGLRendere.h>


namespace CoreEngine
{

	namespace Render
	{
		bool Render::isInit = false;

		Render::Render()
		{
			CORE_ASSERT(isInit, "Render already create");
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

		void Render::RenderPipelineProxy(const DArray<PrimitiveProxy*>& Primitives)
		{
			for (auto* Primitive : Primitives)
			{
				// Return later !!!!!!!!!!!
				//if (StaticMehsProxy* StaticProxy = dynamic_cast<StaticMehsProxy*>(Primitive))
				//{
				//	RenderStaticMeshProxy(StaticProxy);
				//}
				RenderProxy(Primitive);
			}
		}

		

		
	}
}