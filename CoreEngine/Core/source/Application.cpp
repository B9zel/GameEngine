#include <Core/includes/Application.h>
#include <Core/includes/Base.h>


namespace CoreEngine
{
	Application* Application::m_Instance = nullptr;

	Application::Application(const ApplicationOptions& options)
	{
		CORE_ASSERT(m_Instance == nullptr, "Already create applicaton");	
		m_Instance = this;


		
	}
	Application::~Application()
	{
	}

	void Application::CreateApplication(const ApplicationOptions& options)
	{

	}


}

