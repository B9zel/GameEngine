#include <Platform/Windows/includes/WindowsWindow.h>

namespace CoreEngine
{
    static void GLFWErrorCallback(int error_code, const char* description)
    {
        EG_LOG(CORE, ELevelLog::ERROR, "GLFW Code: {0}, Description: {1}", error_code, description);
    }



    WindowsWindow::WindowsWindow(const WindowOptions& options) 
    {
        Init(options);
    }

    String WindowsWindow::GetTitle() const
    {
        return winData.Title;
    }
    uint32 WindowsWindow::GetWidth() const
    {
        return winData.Width;
    }
    uint32 WindowsWindow::GetHeight() const
    {
        return winData.Width;
    }
    void* WindowsWindow::GetNativeWindow() const
    {
        return m_Window;
    }

    void WindowsWindow::OnUpdate()
    {
        m_context->SwapBuffer();
        glfwPollEvents();
        
        glClear(GL_COLOR_BUFFER_BIT);
    }


    void WindowsWindow::Init(const WindowOptions& options)
    {
        winData.Height = options.Height;
        winData.Width = options.Width;
        winData.Title = options.Title;
        
        if (!glfwInit())
        {
            EG_LOG(CORE, ELevelLog::CRITICAL, "Can't initialize GLFW");
            return;
        }
        EG_LOG(CORE, ELevelLog::INFO, "Initialize GLFW");
           

        m_Window = glfwCreateWindow(winData.Width, winData.Height, winData.Title.c_str(), nullptr, nullptr);
        CORE_ASSERT(!m_Window, "Can't create GLFW window");
        EG_LOG(CORE, ELevelLog::INFO, "Create GLFW window");


        m_context = Render::GraphicsContext::CreateContext(m_Window);
        m_context->Init();


        glfwSetErrorCallback(GLFWErrorCallback);


        glfwSetWindowUserPointer(m_Window, &winData);
        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            const WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            switch (action)
            {
                case GLFW_PRESS:
                {
                    EventKeyboardPressed event(key);
                    data->eventCallBack(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    EventKeyboardReleased event(key);
                    data->eventCallBack(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    EventKeyboardRepeat event(key);
                    data->eventCallBack(event);
                    break;
                }
            }
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            const WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            switch (action)
            {
                case GLFW_PRESS:
                {
                    EventMouseButtonPressed event(button);
                    data->eventCallBack(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    EventMouseButtonReleased event(button);
                    data->eventCallBack(event);
                    break;
                }
            }
        });
        
        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) 
            {
                const WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                EventMouseMotion event(xpos, ypos);
                data->eventCallBack(event);
            });
        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) 
            {
                const WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                EventMouseScroll event(xoffset, yoffset);
                data->eventCallBack(event);
            });
        glfwSetCursorEnterCallback(m_Window, [](GLFWwindow* window, int entered) 
            {
                const WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                EventFocuseWindow event(entered);
                data->eventCallBack(event);
            });
        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) 
            {
                const WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                EventCloseWindow event;
                data->eventCallBack(event);
            });
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
            {
                WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                glViewport(0, 0, width, height);

                data->Width = width;
                data->Height = height;
                EventResizeWidnow event(width, height);
                data->eventCallBack(event);
            });


        EG_LOG(CORE, ELevelLog::INFO, "Create window");
    }

}
