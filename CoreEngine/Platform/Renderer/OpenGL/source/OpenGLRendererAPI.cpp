#include <Platform/Renderer/OpenGL/include/OpenGLRendererAPI.h>


namespace CoreEngine
{
	namespace Render
	{
		OpenGLRendererAPI::OpenGLRendererAPI()
		{
			EG_LOG(CORE, ELevelLog::INFO, "Create OpenGL renderere API");
			Init();
		}
		void OpenGLRendererAPI::Init()
		{
			EG_LOG(CORE, ELevelLog::INFO, "Init OpenGL renderere API");
			glClearColor(0, 0, 0, 0);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_TEXTURE_2D);
		}
	}
}