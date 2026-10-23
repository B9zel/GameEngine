#include <Runtime/includes/ActorComponent.h>
#include <Core/includes/World.h>

ActorComponent::ActorComponent(const CoreEngine::InitializeObject& InitParam) : Object(InitParam)
{
}

void ActorComponent::BeginPlay()
{
}
void ActorComponent::UpdateComponent(float deltaTime)
{
}

void ActorComponent::DestroyComponent()
{
	UnregisterComponent();
	MarkGarbage();
}

void ActorComponent::InitProperties()
{
	Object::InitProperties();

	updateFunc.SetUpdateMethod(&ActorComponent::UpdateComponent, this);
}

void ActorComponent::EndPlay()
{


}

void ActorComponent::UnregisterComponent()
{
	if (!isRegistered) return;

	GetWorld()->GetUpdateManager()->RemoveFunction(&updateFunc);
	isRegistered = false;
}

void ActorComponent::RegisteredComponent()
{
	if (isRegistered) return;

	GetWorld()->GetUpdateManager()->AddFunction(&updateFunc);
	isRegistered = true;
}
void ActorComponent::PreRegisterComponent()
{
	if (!GetOwner()->GetIsRegister())
	{
		IsCreatedNative = true;
	}
}
void ActorComponent::SetOwner(Actor* Owner)
{
	this->Owner = Owner;
}

Actor* ActorComponent::GetOwner() const
{
	return Owner;
}
bool ActorComponent::GetIsActive() const
{
	return isActivate;
}
bool ActorComponent::GetIsCreatedNative() const
{
	return IsCreatedNative;
}
