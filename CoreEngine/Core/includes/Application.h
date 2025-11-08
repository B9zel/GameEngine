#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Core/includes/Engine.h>
#include <Core/includes/ShaderLibrary.h>
#include <Core/includes/LayerStack.h>
#include <Events/include/Event.h>
#include <Core/includes/Base.h>



namespace CoreEngine
{
	class Engine;
	class Window;
	class Layer;
	class Event;

	struct ApplicationOptions
	{
		ApplicationOptions() : applicationName{ "" }, pathToApp{ "" }, EngineInstance{ nullptr } {}
		ApplicationOptions(const String appName, const String path, Engine* engine) : applicationName{ appName }, pathToApp{ path }, EngineInstance{ engine }
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
		UniquePtr<Engine> EngineInstance;
	};

	class Application
	{
	public:

		Application(ApplicationOptions& options);
		virtual ~Application() = default;

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

	public:

		static Application* Get() { return m_Instance; }

		const ApplicationOptions& GetAppOptions() const { return appOptions; }

		Window& GetWindow() const { return *window; }
		const UniquePtr<Engine>& GetEngine() const { return InstanceEngine; }
		virtual void Start();
		virtual void OnEvent(Event& event);
		virtual void CreateApp();

		//void PushLayer(Layer* layer) { m_stack.PushLayer(layer); }

	protected:

		void Exit(Event& event);
		virtual void ConstructEngine();

	protected:

		ApplicationOptions appOptions;
		UniquePtr<Window> window;
		UniquePtr<Engine> InstanceEngine;

		//LayerStack m_stack;
		ShaderLibrary shaderLibrary;
		EventDispatch EventDispatcher;

		bool m_isRun;
		static Application* m_Instance;

	private:

		//friend int ::main(int argc, char** argv);
	};
}