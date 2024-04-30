#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Events/include/Event.h>
#include "Window.h"
#include "Base.h"
#include "Dispatcher.h"
#include "Input.h"
#include "LayerStack.h"



int main(int argc, char** argv);

namespace CoreEngine
{
	struct ApplicationOptions
	{
		ApplicationOptions() : applicationName{""}, pathToApp{""} {}
		ApplicationOptions(const String appName,const String path) : applicationName{appName}, pathToApp{path} {}
		String applicationName;
		String pathToApp;
	};

	class Application
	{
	public:


		Application(const ApplicationOptions& options);
		virtual ~Application() {}

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

	public:

		static Application* Get() { return m_Instance; }
		
		Window& GetWindow() const { return *m_window; }
		virtual void Start();
		virtual void OnEvent(Event& event);

	protected:

		void Exit(Event& event);
		
	private:

		ApplicationOptions m_appOptions;
		UniquePtr<Window> m_window;

		LayerStack m_stack;
		EventDispatch m_EventDispatch;
		Input m_Input;

		bool m_isRun;
		

		static Application* m_Instance;

	private:

		friend int ::main(int argc, char** argv);
	};


}