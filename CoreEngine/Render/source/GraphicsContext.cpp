#include <Render/includes/GraphicsContext.h>


namespace CoreEngine
{

	UniquePtr<GraphicsContext> CoreEngine::GraphicsContext::CreateContext()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			EG_LOG(CORE, ELevelLog::CRITICAL, "No renderer API to create");
			return nullptr;
//		cas
		}


		return UniquePtr<GraphicsContext>();
	}
}