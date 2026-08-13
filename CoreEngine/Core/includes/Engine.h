#pragma once
#include <Runtime/CoreObject/Include/Object.h>
#include <Core/includes/ObjectPtr.h>
#include <Core/includes/Base.h>
#include <Math/includes/Vector.h>
#include <Core/includes/AssetManager.h>
#include <Engine.generated.h>

class World;
class Object;

namespace CoreEngine
{
	namespace Reflection
	{
		class ReflectionManager;
	}
	namespace Render
	{
		class Render;
		class RenderDevice;
	} // namespace Render
	class SaveManager;
	class InputDevice;
	class MemoryManager;
	class UpdateManager;
	class TimerManager;
	class Event;

} // namespace CoreEngine
// Main class, that manage all Managers and subsystems
RCLASS()
class Engine : public Object
{

	GENERATED_BODY()

public:

	using ThisClass = Engine;

public:

	/*
	 * Update all classes
	 */
	virtual void Update();
	Engine(const CoreEngine::InitializeObject& Initilize);
	virtual ~Engine();

public:

	/*
	 *  Create singleton class Engine
	 *  @return Instance of Engine class
	 */
	static Engine* Create();

	static Engine* Get();

	UniquePtr<CoreEngine::InputDevice>& GetInputDevice() const;
	UniquePtr<CoreEngine::TimerManager>& GetTimerManager() const;
	UniquePtr<CoreEngine::MemoryManager>& GetMemoryManager() const;
	UniquePtr<CoreEngine::Render::Render>& GetRender() const;
	const UniquePtr<CoreEngine::Render::RenderDevice>& GetRenderDevice() const;
	World* GetWorld() const;
	UniquePtr<CoreEngine::Reflection::ReflectionManager>& GetReflectionManger() const;
	AssetManager* GetAssetManager() const;

	FVector2 GetScreenSize() const;

	virtual void PostInitialize();
	virtual void Init();
	void ConstructInitialize();

	void TakeInputEvent(CoreEngine::Event& Input);

protected:

	virtual World* CreateWorld() const;

protected:

	mutable World* m_World;

private:

	mutable UniquePtr<CoreEngine::InputDevice> m_Input;
	mutable UniquePtr<CoreEngine::MemoryManager> m_MemoryManager;
	mutable UniquePtr<CoreEngine::Render::Render> m_Render;

	mutable UniquePtr<CoreEngine::TimerManager> m_TimerManager;

	RPROPERTY();
	AssetManager* m_AssetManager{nullptr};

	static Engine* GEngine;
};
