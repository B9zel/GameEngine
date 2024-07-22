
#include <Core/includes/Application.h>
#include <Platform/Renderer/OpenGL/include/OpengGLShader.h>

namespace CoreEngine
{
	Application* Application::m_Instance = nullptr;

	Application::Application(const ApplicationOptions& options)
	{
		CORE_ASSERT(m_Instance, "Already create applicaton");	
		m_Instance = this;

		m_appOptions.applicationName = options.applicationName;
		m_appOptions.pathToApp = options.pathToApp;
		
		Log::Init();
		
		m_timerManager = MakeUniquePtr<TimerManager>();

		m_memoryManager.reset(MemoryManager::Create());

		m_Input = MakeUniquePtr<InputDevice>();
	
		m_window = Window::CreateWindow(CoreEngine::WindowOptions(m_appOptions.applicationName, 800, 400));
		m_window->SetEventBind(Function<void(Event&)>(&Application::OnEvent, this));
		
		m_render = Render::Render::Create();


		m_EventDispatch.AddEvent<EventCloseWindow>(BIND_EVENT(&Application::Exit, this));
	}

	void Application::Start()
	{
		float lastTime = 0.f;
		float currentTime = 0.f;
		float delta = 0.f;
		while (m_isRun)
		{
			currentTime = glfwGetTime();
			delta = currentTime - lastTime;
			lastTime = currentTime;
			
			GetTimerManager()->Update(delta);
			m_stack.NativeUpdateAll(delta);
			
			m_window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& event)
	{
		m_EventDispatch.Dispatch<EventCloseWindow>(event);		
		m_Input->InviteEvent(event);
	}

	void Application::Exit(Event& event)
	{
		m_isRun = false;
		glfwTerminate();
	}
}

