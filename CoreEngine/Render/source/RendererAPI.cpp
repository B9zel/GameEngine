#include <Render/includes/RendererAPI.h>


namespace CoreEngine
{
    RendererAPI::API m_Api = RendererAPI::API::OpenGL;
     
    UniquePtr<RendererAPI> RendererAPI::CreateAPI()
    {
        switch (GetAPI())
        {
        case API::None:
            EG_LOG(CORE, ELevelLog::CRITICAL, "No renderer API to create");
            return nullptr;
       // case API::OpenGL:
           // return UniquePtr<RendererAPI>;
        }
        UniquePtr<RendererAPI>();
    }

    RendererAPI::API RendererAPI::GetAPI()
    {
        return m_Api;
    }
}