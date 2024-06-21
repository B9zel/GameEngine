#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Events/include/Event.h>
#include <Render/includes/Render.h>
#include "ShaderLibrary.h"
#include "Window.h"
#include "Base.h"
#include "Dispatcher.h"
#include "InputDevice.h"
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
		UniquePtr<InputDevice>& GetInputDevice() { return m_Input; }
		
		Window& GetWindow() const { return *m_window; }
		virtual void Start();
		virtual void OnEvent(Event& event);

		void PushLayer(Layer* layer) { m_stack.PushLayer(layer); }

	protected:

		void Exit(Event& event);
		
	private:

		ApplicationOptions m_appOptions;
		UniquePtr<Window> m_window;
		UniquePtr<Render::Render> m_render;

		LayerStack m_stack;
		ShaderLibrary m_shaderLibray;
		EventDispatch m_EventDispatch;
		UniquePtr<InputDevice> m_Input;

		bool m_isRun;
		

		static Application* m_Instance;

	private:

		friend int ::main(int argc, char** argv);
	};


}