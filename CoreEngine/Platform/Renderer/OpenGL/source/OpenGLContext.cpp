#include <Platform/Render/OpenGL/include/OpenGLContext.h>


namespace CoreEngine
{

	void OpenGLContext::Init()
	{
		CORE_ASSERT(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Can't initialize GLAD");
		EG_LOG(CORE, ELevelLog::INFO, "Initialize GLAD");
		
		glfwMakeContextCurrent(m_Window);
	}

}