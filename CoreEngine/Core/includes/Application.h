#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Events/include/Event.h>
#include <Render/includes/Render.h>
#include "ShaderLibrary.h"
#include "Window.h"
#include "Base.h"
#include "Dispatcher.h"
#include "InputDevice.h"
#include "LayerStack.h"
#include "MemoryManager.h"
#include "TimerManager.h"



int main(int argc, char** argv);

namespace CoreEngine
{
	struct ApplicationOptions
	{
		ApplicationOptions() : applicationName{""}, pathToApp{""} {}
		ApplicationOptions(const String appName,const String path) : applicationName{appName}, pathToApp{path} 
		{
			const String projectDirectName = "GameEngine";
			size_t pos = path.find(projectDirectName);
			if (pos == String::npos)
			{
				// Application must be in directory "GameEngine"
				throw std::exception("Application must be in directory \"GameEngine\"");
			}
			pathToProject = (pathToApp.substr(0, pos + projectDirectName.size()));
		}
		String applicationName;
		String pathToApp;
		String pathToProject;
	};

	class Application
	{
	public:

		Application(const ApplicationOptions& options);
		virtual ~Application() {}

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

	public:

		static Application* Get() { return m_Instance; }
		UniquePtr<InputDevice>& GetInputDevice() { return m_Input; }
		UniquePtr<class MemoryManager>& GetMamoryManager() { return m_memoryManager; }
		UniquePtr<class TimerManager>& GetTimerManager() { return m_timerManager; }

		const ApplicationOptions& GetAppOptions() const { return m_appOptions; }

		Window& GetWindow() const { return *m_window; }
		virtual void Start();
		virtual void OnEvent(Event& event);

		void PushLayer(Layer* layer) { m_stack.PushLayer(layer); }

	protected:

		void Exit(Event& event);
		
	private:

		ApplicationOptions m_appOptions;
		UniquePtr<Window> m_window;
		UniquePtr<Render::Render> m_render;

		LayerStack m_stack;
		ShaderLibrary m_shaderLibray;
		EventDispatch m_EventDispatch;
		UniquePtr<InputDevice> m_Input;
		UniquePtr<class MemoryManager> m_memoryManager;
		UniquePtr<class TimerManager> m_timerManager;

		bool m_isRun;

		static Application* m_Instance;

	private:

		friend int ::main(int argc, char** argv);
	};



}