#pragma once
#include <Core/includes/Window.h>
#include <Render/includes/GraphicsContext.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>



namespace CoreEngine
{
	class WindowsWindow : public CoreEngine::Window
	{
	public:
		
		WindowsWindow(const WindowOptions& options);

	public:

		virtual String GetTitle() const override;

		virtual unsigned int GetWidth() const override;

		virtual unsigned int GetHeight() const override;

		virtual void* GetNativeWindow() const override;

		virtual void OnUpdate() override;	
		
	protected:

		virtual void Init(const WindowOptions& options) override;

	private:

		UniquePtr<Render::GraphicsContext> m_context;

		GLFWwindow* m_Window;
		
	};

}
