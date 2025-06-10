#include <Render/includes/ElementBufferObject.h>
#include <Render/includes/RendererAPI.h>
#include <Platform/Renderer/OpenGL/include/OpenGLElementBufferObject.h>

namespace CoreEngine
{
	namespace Render
	{
		DECLARE_LOG_CATEGORY_EXTERN(VERTEX_ARR_OBJECT);


		UniquePtr<ElementBufferObject> ElementBufferObject::CreateElementBuffer()
		{
			switch (RendererAPI::GetAPI())
			{
			case RendererAPI::API::None:
				EG_LOG(CORE, ELevelLog::CRITICAL, "No renderer API to create");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return MakeUniquePtr<OpenGL::OpenGLElementBufferObject>();
			default:
				break;
			}
			EG_LOG(CORE, ELevelLog::CRITICAL, "No implament API to create");
		}
	}
}