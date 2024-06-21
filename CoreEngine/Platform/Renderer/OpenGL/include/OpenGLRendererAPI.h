#pragma once
#include <Render/includes/RendererAPI.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace CoreEngine
{
	namespace Render
	{
		class OpenGLRendererAPI : public RendererAPI
		{
		public:

			OpenGLRendererAPI();
		protected:

			void Init();

		private:


		};
	}
}