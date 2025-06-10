#include <Render/includes/VertexArrayObject.h>
#include <Render/includes/RendererAPI.h>
#include <Render/includes/VertextBufferObject.h>
#include <Render/includes/ElementBufferObject.h>
#include <Platform/Renderer/OpenGL/include/OpenGLVertexArrayObject.h>

namespace CoreEngine
{
	namespace Render
	{
		DECLARE_LOG_CATEGORY_EXTERN(VERTEX_ARR_OBJECT);


		UniquePtr<VertexArrayObject> VertexArrayObject::CreateVertexObject()
		{
			switch (RendererAPI::GetAPI())
			{
			case RendererAPI::API::None:
				EG_LOG(CORE, ELevelLog::CRITICAL, "No renderer API to create");
				return nullptr;
			case RendererAPI::API::OpenGL:
				return MakeUniquePtr<OpenGL::OpenGLVertexArrayObject>();
			default:
				break;
			}
			EG_LOG(CORE, ELevelLog::CRITICAL, "No implament API to create");
		}
	}
}