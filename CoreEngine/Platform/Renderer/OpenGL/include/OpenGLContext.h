#pragma once
#include <Render/includes/GraphicsContext.h>

//#include <GLFW/glfw3.h>
#include <Core/includes/Log.h>


struct GLFWwindow;

namespace CoreEngine
{
	namespace Render
	{
		class OpenGLContext : public GraphicsContext
		{
		public:

			OpenGLContext(GLFWwindow* window);

			virtual void Init() override;

			virtual void SwapBuffer() override;

		private:
			GLFWwindow* m_window;

		};
	}
}