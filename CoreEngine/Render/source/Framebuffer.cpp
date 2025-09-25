#include <Render/includes/Framebuffer.h>
#include <Render/includes/RendererAPI.h>
#include <Platform/Renderer/OpenGL/include/OpenGLFramebuffer.h>


namespace CoreEngine
{
	namespace Render
	{
		SharedPtr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& Spec)
		{
			switch (RendererAPI::GetAPI())
			{
			case RendererAPI::API::None:
				EG_LOG(CORE, ELevelLog::CRITICAL, "No renderer API to create");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return MakeSharedPtr<OpenGL::OpenGLFramebuffer>(Spec);
			default:
				break;
			}
			EG_LOG(CORE, ELevelLog::CRITICAL, "No implament API to create");
		}
	}
}