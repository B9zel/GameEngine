#pragma once
#include <Core/includes/Base.h>
#include <Math/includes/Vector.h>
#include <Events/include/Event.h>


namespace CoreEngine
{
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


	// Main class, that manage all Managers and subsystems
	class Engine
	{
	public:

		/*
		* Update all classes
		*/
		void Update();
		virtual ~Engine() = default;

	public:

		/*
		*  Create singleton class Engine
		*  @return Instance of Engine class
		*/
		static UniquePtr<Engine> Create();

		static Engine* Get();

		UniquePtr<InputDevice>& GetInputDevice() { return m_Input; }
		UniquePtr<TimerManager>& GetTimerManager() { return m_TimerManager; }
		UniquePtr<MemoryManager>& GetMemoryManager() { return m_MemoryManager; }
		UniquePtr<Render::Render>& GetRender() { return m_Render; }
		UniquePtr<World>& GetWorld() { return m_World; }

		FVector2 GetScreenSize() const;

		virtual void PostInitialize();

		void TakeInputEvent(Event& Input);

	protected:

		Engine();

	private:

		UniquePtr<InputDevice> m_Input;
		UniquePtr<MemoryManager> m_MemoryManager;
		UniquePtr<Render::Render> m_Render;
		UniquePtr<TimerManager> m_TimerManager;
		UniquePtr<class World> m_World;

		static Engine* GEngine;
	};
}