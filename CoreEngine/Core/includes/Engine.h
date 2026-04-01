#pragma once
#include <Runtime/CoreObject/Include/Object.h>
#include <Core/includes/ObjectPtr.h>
#include <Core/includes/Base.h>
#include <Math/includes/Vector.h>
#include <Core/includes/AssetManager.h>
#include <Engine.generated.h>

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
	namespace Runtime
	{
		class Object;
	}
	class SaveManager;
	class InputDevice;
	class MemoryManager;
	class UpdateManager;
	class TimerManager;
	class Engine;
	class World;
	class Event;
	class AssetManager;

	// Main class, that manage all Managers and subsystems
	RCLASS()
	class Engine : public CoreEngine::Runtime::Object
	{

		GENERATED_BODY()

	public:

		using ThisClass = Engine;

	public:

		/*
		 * Update all classes
		 */
		virtual void Update();
		Engine(const InitializeObject& Initilize);
		virtual ~Engine() = default;

	public:

		/*
		 *  Create singleton class Engine
		 *  @return Instance of Engine class
		 */
		static Engine* Create();

		static Engine* Get();

		UniquePtr<InputDevice>& GetInputDevice() const;
		UniquePtr<TimerManager>& GetTimerManager() const;
		UniquePtr<MemoryManager>& GetMemoryManager() const;
		UniquePtr<Render::Render>& GetRender() const;
		const UniquePtr<Render::RenderDevice>& GetRenderDevice() const;
		World* GetWorld() const;
		UniquePtr<Reflection::ReflectionManager>& GetReflectionManger() const;
		AssetManager* GetAssetManager() const;

		FVector2 GetScreenSize() const;

		virtual void PostInitialize();
		virtual void Init();
		void ConstructInitialize();

		void TakeInputEvent(Event& Input);

	protected:

		virtual World* CreateWorld() const;

	protected:

		mutable World* m_World;

	private:

		mutable UniquePtr<InputDevice> m_Input;
		mutable UniquePtr<MemoryManager> m_MemoryManager;
		mutable UniquePtr<Render::Render> m_Render;

		mutable UniquePtr<TimerManager> m_TimerManager;

		

		RPROPERTY();
		AssetManager* m_AssetManager{nullptr};

		static Engine* GEngine;
	};
} // namespace CoreEngine
