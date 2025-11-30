#include <Core/includes/Application.h>

#include <Core/includes/Base.h>
#include <Core/includes/Window.h>
#include <Core/includes/World.h>
#include <Core/includes/Dispatcher.h>





namespace CoreEngine
{
	Application* Application::m_Instance = nullptr;

	Application::Application(ApplicationOptions& options)
	{

		appOptions.applicationName = options.applicationName;
		appOptions.pathToApp = options.pathToApp;
		appOptions.pathToProject = options.pathToProject;
		//m_Engine = move(options.EngineInstance);


	}

	void Application::Start()
	{
		InstanceEngine->PostInitialize();
		while (m_isRun)
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

		ConstructEngine();


		EventDispatcher.AddEvent<EventCloseWindow>(BIND_EVENT(&Application::ExitInput, this));
	}

	void Application::ExitInput(Event& event)
	{
		Exit();
	}

	void Application::Exit()
	{
		m_isRun = false;
		glfwTerminate();
	}

	void Application::ConstructEngine()
	{
		if (InstanceEngine) return;
		InstanceEngine = MakeUniquePtr<Engine>();
	}

}