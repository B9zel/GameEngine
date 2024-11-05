#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Core/includes/ShaderLibrary.h>
#include <Core/includes/LayerStack.h>
#include <Events/include/Event.h>
#include <Core/includes/Engine.h>
#include "Base.h"



int32 main(int32 argc, char** argv);

namespace CoreEngine
{

	class Engine;
	class Window;
	class Layer;
	class Event;


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
		virtual ~Application() = default;

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

	public:

		static Application* Get() { return m_Instance; }
		
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
		UniquePtr<Engine> m_Engine;


		LayerStack m_stack;
		ShaderLibrary m_shaderLibray;
		EventDispatch m_EventDispatch;
		
	
		bool m_isRun;

		static Application* m_Instance;

	private:

		friend int ::main(int argc, char** argv);
	};



}