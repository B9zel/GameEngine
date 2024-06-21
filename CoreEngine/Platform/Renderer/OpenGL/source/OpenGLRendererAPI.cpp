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
		}
	}
}