#pragma once
#include <Core/includes/Window.h>
#include <Core/includes/Base.h>
#include <GLFW/glfw3.h>

int main(int argc, char** argv);

namespace CoreEngine
{
	struct ApplicationOptions
	{
		
	};

	class Application
	{
	public:

		Application(const ApplicationOptions& options);
		virtual ~Application();

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

	public:

		void CreateApplication(const ApplicationOptions& options);


	protected:

		ApplicationOptions appOptions;
		bool isRun;

	private:

		static Application* m_Instance;

	private:

		friend int ::main(int argc, char** argv);
	};


}