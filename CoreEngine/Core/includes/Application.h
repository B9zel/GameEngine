#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Core/includes/ShaderLibrary.h>
#include <Core/includes/LayerStack.h>
#include <Events/include/Event.h>
#include <Core/includes/Base.h>

class Engine;

namespace CoreEngine
{
	namespace Reflection
	{
		class ReflectionManager;
	}

	class Window;
	class Layer;
	class Event;

	struct ApplicationOptions
	{
		ApplicationOptions();
		ApplicationOptions(const String appName, const String path, Engine* engine);
		String applicationName;
		String pathToApp;
		String pathToProject;
		UniquePtr<class Engine> EngineInstance;
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

		static Application* Get()
		{
			return m_Instance;
		}

		const ApplicationOptions& GetAppOptions() const
		{
			return appOptions;
		}

		Window& GetWindow() const
		{
			return *window;
		}
		const UniquePtr<Engine>& GetEngine() const
		{
			return InstanceEngine;
		}
		UniquePtr<Reflection::ReflectionManager>& GetReflectionManager() const
		{
			return m_ReflectionManger;
		}
		virtual void Start();
		virtual void OnEvent(Event& event);
		virtual void CreateApp();

		virtual void Exit();

	protected:

		void ExitInput(Event& event);
		virtual void ConstructEngine();

	protected:

		ApplicationOptions appOptions;
		UniquePtr<Window> window;
		UniquePtr<Engine> InstanceEngine;
		mutable UniquePtr<Reflection::ReflectionManager> m_ReflectionManger;

		// LayerStack m_stack;
		ShaderLibrary shaderLibrary;
		EventDispatch EventDispatcher;

		bool m_isRun;
		static Application* m_Instance;

	private:

		// friend int ::main(int argc, char** argv);
	};
} // namespace CoreEngine
