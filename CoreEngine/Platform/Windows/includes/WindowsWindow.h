#pragma once
#include <Core/includes/Window.h>
#include <GLFW/glfw3.h>



namespace CoreEngine
{
	class WindowsWindow : public CoreEngine::Window
	{
	public:
		
		WindowsWindow(const WindowOptions& options);

		virtual String GetTitle() const override;

		virtual unsigned int GetWidth() const override;

		virtual unsigned int GetHeight() const override;

		virtual void* GetNativeWindow() const override;

		virtual void OnUpdate() override;

	private:

		virtual void Init(const WindowOptions& options);

	private:
		GLFWwindow* m_Window;
		
	};

}
