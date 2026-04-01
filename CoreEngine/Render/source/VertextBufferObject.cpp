#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#include <Render/includes/VertextBufferObject.h>
#include <Render/includes/RendererAPI.h>
#include <Platform/Renderer/OpenGL/include/OpenGLVertextBufferObject.h>

namespace CoreEngine
{
	namespace Render
	{
		UniquePtr<VertexBufferObject> VertexBufferObject::CreateVertexBufferObject()
		{
			switch (RendererAPI::GetAPI())
			{
			case RendererAPI::API::None:
				EG_LOG(CORE, ELevelLog::CRITICAL, "No renderer API to create");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return MakeUniquePtr<OpenGL::OpenGLVertexBufferObject>();
			default:
				break;
			}
			ASSERT("No implament API to create");
		}
	} // namespace Render
} // namespace CoreEngine
