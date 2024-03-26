#pragma once
#include <Core/includes/Base.h>

namespace CoreEngine
{
	struct WindowOptions
	{
		String Title = "";
		unsigned int Width = 0;
		unsigned int Height = 0;
	};


	class Window
	{
	public:

		virtual ~Window() = 0;

		virtual void OnUpdate() = 0;

		static UniquePtr<Window> CreateWindow(const WindowOptions& options);
		
		virtual unsigned int GetWidth() const = 0;

		virtual unsigned int GetHeight() const = 0;

		virtual String GetTitle() const = 0;

		virtual void* GetNativeWindow() const = 0;

		virtual bool GetIsCreateWindow() const;

		const WindowOptions& GetWindowOptions() const;

	protected:

		WindowOptions Options;
		bool isCreateWindow;
	};
}