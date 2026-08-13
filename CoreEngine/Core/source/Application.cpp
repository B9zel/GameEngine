#include <Core/includes/Application.h>

#include <Core/includes/Engine.h>
#include <Core/includes/Base.h>
#include <Core/includes/Window.h>
#include <Core/includes/World.h>
#include <Core/includes/Dispatcher.h>

namespace CoreEngine
{

	ApplicationOptions::ApplicationOptions() : applicationName{""}, pathToApp{""}, EngineInstance{nullptr}
	{
	}

	ApplicationOptions::ApplicationOptions(const String appName, const String path, Engine* engine)
		: applicationName{appName}, pathToApp{path}, EngineInstance{engine}
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
} // namespace CoreEngine

namespace CoreEngine
{
	Application* Application::m_Instance = nullptr;

	Application::Application(ApplicationOptions& options)
	{

		appOptions.applicationName = options.applicationName;
		appOptions.pathToApp = options.pathToApp;
		appOptions.pathToProject = options.pathToProject;
		// m_Engine = move(options.EngineInstance);
	}

	Application::~Application()
	{
		InstanceEngine.reset();
		m_ReflectionManger.reset();
		window.reset();
		m_Instance = nullptr;
		glfwTerminate();
	}
	void Application::Start()
	{
		InstanceEngine->PostInitialize();
		while (m_isRun && window && !glfwWindowShouldClose(static_cast<GLFWwindow*>(window->GetNativeWindow())))
		{
			InstanceEngine->Update();
			window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher.Dispatch<EventCloseWindow>(event);
		InstanceEngine->TakeInputEvent(event);
	}

	void Application::CreateApp()
	{
		CORE_UNASSERT(m_Instance, "Already create application");
		m_Instance = this;

		Log::Init();

		window = Window::CreateWindow(CoreEngine::WindowOptions(appOptions.applicationName, 800, 400));
		window->SetEventBind(Function<void(Event&)>(&Application::OnEvent, this));

		m_ReflectionManger = std::move(Reflection::ReflectionManager::CreateReflectionManager());

		ConstructEngine();
		InstanceEngine->ConstructInitialize();
		InstanceEngine->Init();

		EventDispatcher.AddEvent<EventCloseWindow>(BIND_EVENT(&Application::ExitInput, this));
	}

	void Application::ExitInput(Event& event)
	{
		Exit();
	}

	void Application::Exit()
	{
		m_isRun = false;
	}

	void Application::ConstructEngine()
	{
		if (InstanceEngine) return;
		InitializeObject InitParam;
		InitParam.Class = Engine::GetStaticClass();

		InstanceEngine = MakeUniquePtr<Engine>(InitParam);
	}

} // namespace CoreEngine
