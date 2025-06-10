#include <Render/includes/ShaderStorageBufferObject.h>
#include <Platform/Renderer/OpenGL/include/OpenGLShaderStorageBufferObject.h>
#include <Render/includes/RendererAPI.h>


namespace CoreEngine
{
	namespace Render
	{
		UniquePtr<ShaderStorageBufferObject> ShaderStorageBufferObject::CreateShaderStorageBufferObject()
		{
			switch (RendererAPI::GetAPI())
			{
			case RendererAPI::API::None:
				EG_LOG(CORE, ELevelLog::CRITICAL, "No renderer API to create");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return MakeUniquePtr<OpenGL::OpenGLShaderStorageBufferObject>();
			default:
				break;
			}
			EG_LOG(CORE, ELevelLog::CRITICAL, "No implament API to create");
		}
	}
}