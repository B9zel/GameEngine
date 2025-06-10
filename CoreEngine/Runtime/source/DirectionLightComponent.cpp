
#include <Runtime/includes/DirectionLightComponent.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

#include <Core/includes/LightProxy.h>

#include "Math/includes/Math.h"

namespace CoreEngine
{
	namespace Runtime
	{
		DirectionLightComponent::DirectionLightComponent()
		{
			LightProxy = MakeUniquePtr<DirectionLightProxy>();
		}
		LightProxy* DirectionLightComponent::GetLightProxy()
		{
			const FVector4& Rotat = FVector4((GetComponentRotation()), 0);
			//const FVector& Div = (GetComponentRotation() / 360);
			/*const float x = Rotat.GetX() > 360 ? Rotat.GetX() - Rotat.GetX() * (Rotat.GetX() / 360) : Rotat.GetX();
			const float y = Rotat.GetY() > 360 ? Rotat.GetY() - Rotat.GetY() * (Rotat.GetY() / 360) : Rotat.GetY();
			const float z = Rotat.GetZ() > 360 ? Rotat.GetZ() - Rotat.GetZ() * (Rotat.GetZ() / 360) : Rotat.GetZ();*/
			FMatrix4x4 RotMat = glm::yawPitchRoll(Rotat.GetX(), Rotat.GetY(), Rotat.GetZ());
			FVector4 ResDirect = RotMat * Rotat.vector; // FVector4(0, -1, 0, 1);
			//LightProxy->SetDirection(FVector(ResDirect.x, ResDirect.y, ResDirect.z));
			LightProxy->SetDirection(GetForwardVector().SafeNormalize());
			//LightProxy->SetDirection(FVector(-0.2f, -1.0f, -0.3f));
			EG_LOG(CORE, ELevelLog::INFO, "X: {0} Y: {1} Z: {2}", GetForwardVector().GetX(), GetForwardVector().GetY(), GetForwardVector().GetZ());
			LightProxy->SetColor(GetColor());
			LightProxy->SetIntencity(GetIntencity());

			return LightProxy.get();
		}
	}
}