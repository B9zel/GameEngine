#include <Runtime/includes/PrimitiveComponent.h>
#include <Core/includes/PrimitiveProxy.h>
#include <Runtime/includes/Actor.h>

DECLARE_LOG_CATEGORY_EXTERN(PRIMITIVE_COMPONENT);


namespace CoreEngine
{
	namespace Runtime
	{
		PrimitiveComponent::PrimitiveComponent(const InitializeObject& Object) : SceneComponent(Object)
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
			// Actor* owner = GetOwner();
			/*if (sceneProxy)
			{
				sceneProxy->SetTransform(GetTransform());

			}*/

			return sceneProxy;
		}

		PrimitiveProxy* PrimitiveComponent::GetUpdateProxy() const
		{
			/*Transform ProxyTransform = GetTransform();
			ProxyTransform.SetRotation(Math::ToDegreesVector(ProxyTransform.GetRotation()));*/
			sceneProxy->SetTransform(GetTransform());
			return sceneProxy;
		}
	}
}

