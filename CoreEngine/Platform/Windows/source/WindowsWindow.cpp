#include <Platform/Windows/includes/WindowsWindow.h>

namespace CoreEngine
{
    WindowsWindow::WindowsWindow(const WindowOptions& options)
    {
        Options = options;
    }
    String WindowsWindow::GetTitle() const
    {
        return Options.Title;
    }
    unsigned int WindowsWindow::GetWidth() const
    {
        return Options.Width;
    }
    unsigned int WindowsWindow::GetHeight() const
    {
        return Options.Width;
    }
    void* WindowsWindow::GetNativeWindow() const
    {
        return m_Window;
    }

    void WindowsWindow::OnUpdate()
    {
    }

    void WindowsWindow::Init(const WindowOptions& options)
    {
        m_Window = glfwCreateWindow(GetWidth(), GetHeight(), GetTitle().c_str(), nullptr, nullptr);
        CORE_ASSERT(m_Window == nullptr, "Can't create window GLFW");

    }

}
