#include <Runtime/includes/PrimitiveComponent.h>
#include <Core/includes/PrimitiveProxy.h>

DECLARE_LOG_CATEGORY_EXTERN(PRIMITIVE_COMPONENT);


namespace CoreEngine
{
    namespace Runtime
    {
        PrimitiveComponent::PrimitiveComponent()
        {
            try
            {
                sceneProxy = new PrimitiveProxy();
            }
            catch (const std::exception& error)
            {
                EG_LOG(PRIMITIVE_COMPONENT, ELevelLog::ERROR, error.what());
                throw error;
            }
        }
        PrimitiveProxy* CoreEngine::Runtime::PrimitiveComponent::GetSceneProxy() const
        {
            return sceneProxy;
        }
    }
}

