#include <Render/includes/RendererAPI.h>
#include <Platform/Renderer/OpenGL/include/OpenGLRendererAPI.h>

namespace CoreEngine
{
    namespace Render
    {

        RendererAPI::API RendererAPI::m_Api = RendererAPI::API::OpenGL;

        UniquePtr<RendererAPI> RendererAPI::CreateAPI()
        {
            switch (GetAPI())
            {
            case API::None:
                EG_LOG(CORE, ELevelLog::CRITICAL, "No renderer API to create");
                return nullptr;
            case API::OpenGL:
                return MakeUniquePtr<OpenGL::OpenGLRendererAPI>();
            }
            EG_LOG(CORE, ELevelLog::CRITICAL, "API not selected");
        }

        RendererAPI::API RendererAPI::GetAPI()
        {
            return m_Api;
        }
    }
}