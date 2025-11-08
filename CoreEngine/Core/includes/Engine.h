#pragma once
#include <Core/includes/ObjectPtr.h>
#include <Core/includes/Base.h>
#include <Math/includes/Vector.h>



namespace CoreEngine
{
	namespace Reflection
	{
		class ReflectionManager;
	}
	namespace Render
	{
		class Render;
	}
	class InputDevice;
	class MemoryManager;
	class UpdateManager;
	class TimerManager;
	class Engine;
	class World;
	class Event;

	// Main class, that manage all Managers and subsystems
	class Engine
	{
	public:

		using ThisClass = Engine;

	public:

		/*
		* Update all classes
		*/
		virtual void Update();
		Engine();
		virtual ~Engine() = default;

	public:

		/*
		*  Create singleton class Engine
		*  @return Instance of Engine class
		*/
		static UniquePtr<ThisClass> Create();

		static Engine* Get();

		UniquePtr<InputDevice>& GetInputDevice() { return m_Input; }
		UniquePtr<TimerManager>& GetTimerManager() { return m_TimerManager; }
		UniquePtr<MemoryManager>& GetMemoryManager() { return m_MemoryManager; }
		UniquePtr<Render::Render>& GetRender() { return m_Render; }
		World* GetWorld() { return m_World; }
		UniquePtr<Reflection::ReflectionManager>& GetReflectionManger() { return m_ReflectionManger; }

		FVector2 GetScreenSize() const;

		virtual void PostInitialize();

		void TakeInputEvent(Event& Input);

	protected:


	private:

		UniquePtr<InputDevice> m_Input;
		UniquePtr<MemoryManager> m_MemoryManager;
		UniquePtr<Render::Render> m_Render;
		UniquePtr<TimerManager> m_TimerManager;

		World* m_World;
		UniquePtr<class Reflection::ReflectionManager> m_ReflectionManger;

		static Engine* GEngine;
	};
}