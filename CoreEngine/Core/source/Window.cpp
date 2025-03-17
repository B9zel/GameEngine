#include <Core/includes/Window.h>
#include <Core/includes/Base.h>
#ifdef EG_PLATFORM_WINDOWS
	#include <Platform/Windows/includes/WindowsWindow.h>
#endif


namespace CoreEngine
{
	UniquePtr<Window> Window::CreateWindow(const WindowOptions& options)
	{
#ifdef EG_PLATFORM_WINDOWS
		return MakeUniquePtr<WindowsWindow>(options);
#else
		#error "Unknown platform"
#endif // EG_PLATFORM_WINDOWS
	}

	bool Window::GetIsCreateWindow() const
	{
		return isCreateWindow;
	}
}