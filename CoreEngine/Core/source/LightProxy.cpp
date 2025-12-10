#include <Core/includes/LightProxy.h>

#include <Math/includes/Math.h>

namespace CoreEngine
{
	const FVector& LightProxy::GetColor()
	{
		return Color;
	}
	float LightProxy::GetIntencity() const
	{
		return Intencity;
	}
	int LightProxy::GetID() const
	{
		return IDLight;
	}
	void LightProxy::SetColor(const FVector& NewColor)
	{
		Color = NewColor;
	}
	void LightProxy::SetIntencity(const float NewIntencity)
	{
		Intencity = NewIntencity;
	}
	void LightProxy::SetID(const int NewID)
	{
		IDLight = NewID;
	}
}

namespace CoreEngine
{
	void DirectionLightProxy::SetDirection(const FVector& NewDirection)
	{
		m_Direction = NewDirection;
	}
	const FVector& DirectionLightProxy::GetDirection() const
	{
		return m_Direction;
	}
	const ETypeLight DirectionLightProxy::GetTypeLight() const
	{
		return ETypeLight::DIRECTION_LIGHT;
	}
}

namespace CoreEngine
{
	void PointLightProxy::SetLocation(const FVector& NewLocation)
	{
		m_Location = NewLocation;
	}

	void PointLightProxy::SetConstant(const float NewConstant)
	{
		m_Constant = NewConstant;
	}

	void PointLightProxy::SetLinear(const float NewLinear)
	{
		m_Linear = NewLinear;
	}

	void PointLightProxy::SetQuadratic(const float NewQuadratic)
	{
		m_Quadratic = NewQuadratic;
	}

	const FVector& PointLightProxy::GetLocation() const
	{
		return m_Location;
	}

	float PointLightProxy::GetConstant() const
	{
		return m_Constant;
	}

	float PointLightProxy::GetLinear() const
	{
		return	m_Linear;
	}

	float PointLightProxy::GetQuadratic() const
	{
		return m_Quadratic;
	}

	const ETypeLight PointLightProxy::GetTypeLight() const
	{
		return ETypeLight::POINT_LIGHT;
	}

}

namespace CoreEngine
{
	void SpotLightProxy::SetLocation(const FVector& NewLocation)
	{
		m_Location = NewLocation;
	}

	void SpotLightProxy::SetDirection(const FVector& NewDirection)
	{
		m_Direction = NewDirection;
	}

	void SpotLightProxy::SetConstant(const float NewConstant)
	{
		m_Constant = NewConstant;
	}

	void SpotLightProxy::SetLinear(const float NewLinear)
	{
		m_Linear = NewLinear;
	}

	void SpotLightProxy::SetQuadratic(const float NewQuadratic)
	{
		m_Quadratic = NewQuadratic;
	}

	void SpotLightProxy::SetCutOff(const float NewCutOff)
	{
		m_CutOff = Math::Max(NewCutOff, 0.0f);
	}

	void SpotLightProxy::SetOuterCutOff(const float NewOuterCutOff)
	{
		m_OuterCutOff = Math::Max(NewOuterCutOff, 0.0f);
	}

	const FVector& SpotLightProxy::GetLocation() const
	{
		return m_Location;
	}

	const FVector& SpotLightProxy::GetDirection() const
	{
		return m_Direction;
	}

	float SpotLightProxy::GetConstant() const
	{
		return m_Constant;
	}

	float SpotLightProxy::GetLinear() const
	{
		return m_Linear;
	}

	float SpotLightProxy::GetQuadratic() const
	{
		return m_Quadratic;
	}

	float SpotLightProxy::GetCutOff() const
	{
		return m_CutOff;
	}

	float SpotLightProxy::GetOuterCutOff() const
	{
		return m_OuterCutOff;
	}

	const ETypeLight SpotLightProxy::GetTypeLight() const
	{
		return ETypeLight::SPOTLIGHT;
	}

}