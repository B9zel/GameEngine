#include <Runtime/includes/CollisionComponent.h>
#include <Math/includes/Quat.h>
#include <Core/includes/World.h>

CollisionComponent::CollisionComponent(const CoreEngine::InitializeObject& Object) : SceneComponent(Object)
{
	ChangeSimulatePhysics.AddBind(&CollisionComponent::OnChangeSimulate, this);
}

void CollisionComponent::UpdateComponent(float deltaTime)
{
	SceneComponent::UpdateComponent(deltaTime);
	EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "{} {} {}", GetVelocity().GetX(), GetVelocity().GetY(), GetVelocity().GetZ());
	if (!GetIsSimulatePhysics())
	{
		m_BodyInstance->SetLocation(GetWorldLocation());
		m_BodyInstance->SetRotation(GetComponentRotation());
	}
}

void CollisionComponent::SetComponentRotation(const FVector& newRotation)
{
	SceneComponent::SetComponentRotation(newRotation);

	m_BodyInstance->SetRotation(newRotation);
}

void CollisionComponent::SetComponentLocation(const FVector& newLocation)
{
	SceneComponent::SetComponentLocation(newLocation);

	m_BodyInstance->SetLocation(newLocation);
}

void CollisionComponent::AddComponentRotation(const FVector& addRotation)
{
	SceneComponent::AddComponentRotation(addRotation);

	m_BodyInstance->SetRotation(GetComponentRotation());
}

void CollisionComponent::AddComponentLocation(const FVector& addLocation)
{
	SceneComponent::AddComponentLocation(addLocation);

	m_BodyInstance->SetLocation(GetComponentLocation());
}

UniquePtr<CoreEngine::Physic::BodyInstance> CollisionComponent::CreateBodyInstance()
{
	throw std::runtime_error("CreateBodyInstance must be overrided");
}

void CollisionComponent::RegisteredComponent()
{
	SceneComponent::RegisteredComponent();

	if (!CoreEngine::Physic::IsValidMass(Mass))
	{
		Mass = 0.0f;
	}
	m_BodyInstance = CreateBodyInstance();
	m_BodyInstance->RegistPhysBody();
	m_BodyInstance->OnCallbackSimulation.AddBind(&CollisionComponent::CallbackSimulation, this);
	GetWorld()->GetPhysScene()->RegisterBody(m_BodyInstance.get());
}

void CollisionComponent::CallbackSimulation(CoreEngine::Physic::CallbackUpdateDataRB Data)
{
	const FMatrix4x4 WorldMatrix = Math::Inverse(MakeParentMatrix()) * Data.transform;
	FVector Location;
	FQuat Rotation;
	if (Math::DecomposeMatrix(WorldMatrix, Location, Rotation, FVector()))
	{
		SceneComponent::SetComponentLocation(Location);
		SceneComponent::SetComponentRotation(Math::ToDegreesVector(Math::ToEuler(Rotation)));
	}
	SetVelocity(Data.Velocity);

	// GetTransform().ToMatrix();

	// SetComponentLocation(Data.Location);
}

void CollisionComponent::OnChangeSimulate(bool IsSimulate)
{
	if (!IsSimulate)
	{
		Velocity = FVector::ZeroVector;
	}
}

void CollisionComponent::SetSimulatePhysics(const bool IsSimulate, const bool ApplyForce)
{
	if (ApplyForce || IsSimulatePhysics != IsSimulate)
	{
		IsSimulatePhysics = IsSimulate;
		m_BodyInstance->SetSimulate(IsSimulate);
		ChangeSimulatePhysics.Call(IsSimulate);
	}
}

bool CollisionComponent::GetIsSimulatePhysics() const
{
	return IsSimulatePhysics;
}

void CollisionComponent::PostEditChangeProperty(CoreEngine::Reflection::PropertyField& Property)
{
	SceneComponent::PostEditChangeProperty(Property);

	if (Property.Name == STRINGCON_DETAILS(IsSimulatePhysics))
	{
		SetSimulatePhysics(IsSimulatePhysics, true);
	}
	if (Property.Name == STRINGCON_DETAILS(Mass))
	{
		if (m_BodyInstance)
		{
			m_BodyInstance->SetMass(Mass);
			Mass = m_BodyInstance->GetDataSettings().mass;
		}
		else if (!CoreEngine::Physic::IsValidMass(Mass))
		{
			Mass = 0.0f;
		}
	}
}
