#include <Platform/Renderer/OpenGL/include/OpenGLContext.h>


#include <glad/glad.h>
#include <GLFW/glfw3.h>



namespace CoreEngine
{
	namespace Render
	{
		OpenGLContext::OpenGLContext(GLFWwindow* window) : m_window(window)
		{
		}

		void OpenGLContext::Init()
		{
			glfwMakeContextCurrent(m_window);

			CORE_ASSERT(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Can't initialize GLAD");
			EG_LOG(CORE, ELevelLog::INFO, "Initialize GLAD\n");


			EG_LOG(CORE, ELevelLog::INFO, "OpenGL info");
			EG_LOG(CORE, ELevelLog::INFO, "Version: {0}", (char*)glGetString(GL_VERSION));
			EG_LOG(CORE, ELevelLog::INFO, "Vendor: {0}", (char*)glGetString(GL_VENDOR));
			EG_LOG(CORE, ELevelLog::INFO, "Renderer: {0}\n", (char*)glGetString(GL_RENDERER));

			glfwSwapInterval(0);
		}

		void OpenGLContext::SwapBuffer()
		{
			glfwSwapBuffers(m_window);
		}
	}
}