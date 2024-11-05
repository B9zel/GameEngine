#include <Render/includes/Render.h>


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
			UniquePtr<Render> render = MakeUniquePtr<Render>();
			render->m_renderAPI = RendererAPI::CreateAPI();
			
			return render;
		}
	}
}