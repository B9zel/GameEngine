#include <Render/includes/Texture.h>
#include <Render/includes/RendererAPI.h>
#include <Platform/Renderer/OpenGL/include/OpenGLTexture.h>


namespace CoreEngine
{
	namespace Render
	{
		SharedPtr<Texture2D> Texture2D::Create(const char* path)
		{
			switch (RendererAPI::GetAPI())
			{
			case RendererAPI::API::None:
				EG_LOG(CORE, ELevelLog::CRITICAL, "No renderer API to create");
				return nullptr;
			case RendererAPI::API::OpenGL: 
				return MakeSharedPtr<OpenGLTexture2D>(path);
			default:
				break;
			}
		}
	}
}
