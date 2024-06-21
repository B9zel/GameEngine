#include <Render/includes/GraphicsContext.h>
#include <Platform/Renderer/OpenGL/include/OpenGLContext.h>
#include <Render/includes/RendererAPI.h>


namespace CoreEngine
{
	namespace Render
	{
		UniquePtr<GraphicsContext> GraphicsContext::CreateContext(void* window)
		{
			switch (RendererAPI::GetAPI())
			{
			case RendererAPI::API::None:
				EG_LOG(CORE, ELevelLog::CRITICAL, "No renderer API to create");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return MakeUniquePtr<OpenGLContext>(static_cast<GLFWwindow*>(window));
			}
		}
	}
}