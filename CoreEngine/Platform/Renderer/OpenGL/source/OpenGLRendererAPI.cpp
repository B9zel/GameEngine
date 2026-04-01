#include <Platform/Renderer/OpenGL/include/OpenGLRendererAPI.h>

namespace CoreEngine
{
	namespace Render
	{
		namespace OpenGL
		{
			OpenGLRendererAPI::OpenGLRendererAPI()
			{
				EG_LOG(CORE, ELevelLog::INFO, "Create OpenGL renderere API");
				Init();
			}
			void OpenGLRendererAPI::Init()
			{
				EG_LOG(CORE, ELevelLog::INFO, "Init OpenGL renderere API");
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
				glClearColor(0, 0, 0, 0);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glEnable(GL_TEXTURE_2D);
				glEnable(GL_DEPTH_TEST);
			}
		} // namespace OpenGL
	} // namespace Render
} // namespace CoreEngine
