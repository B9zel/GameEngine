
#include <Core/includes/Application.h>


namespace CoreEngine
{
	Application* Application::m_Instance = nullptr;

	Application::Application(const ApplicationOptions& options)
	{
		CORE_ASSERT(m_Instance, "Already create applicaton");	
		m_Instance = this;

		m_appOptions.applicationName = options.applicationName;
		m_appOptions.pathToApp = options.pathToApp;
			
		
		m_window = Window::CreateWindow(CoreEngine::WindowOptions(m_appOptions.applicationName, 800, 400));
		m_window->SetEventBind(Function<void(Event&)>(&Application::OnEvent, this));
		
		m_EventDispatch.AddEvent<EventCloseWindow>(BIND_EVENT(&Application::Exit, this));
		
		
		glClearColor(0, 0, 0, 0);
	}

	void Application::Start()
	{
		while (m_isRun)
		{
			m_window->OnUpdate();			
		}
	}

	void Application::OnEvent(Event& event)
	{
		m_EventDispatch.Dispatch<EventCloseWindow>(event);		
		m_Input.InviteEvent(event);
	}

	void Application::Exit(Event& event)
	{
		m_isRun = false;
	}

	


}

