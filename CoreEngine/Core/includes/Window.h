#pragma once
#include <Core/includes/Base.h>
#include <Events/include/Event.h>


namespace CoreEngine
{
	struct WindowOptions
	{
		WindowOptions() {}
		WindowOptions(const String& title, unsigned int widht, unsigned int height) : Title{ title }, Width{widht}, Height{height} {}
		String Title = "";
		unsigned int Width = 0;
		unsigned int Height = 0;
	};


	class Application;
	class Window
	{
		
	public:

		virtual ~Window() = default;

	public:

		using FunctionEventCallBack = Function<void(Event&)>;

	public:
		virtual void OnUpdate() = 0;

		static UniquePtr<Window> CreateWindow(const WindowOptions& options);
		
		virtual unsigned int GetWidth() const = 0;

		virtual unsigned int GetHeight() const = 0;

		virtual String GetTitle() const = 0;

		virtual void* GetNativeWindow() const = 0;

		virtual bool GetIsCreateWindow() const;

		void SetEventBind(const FunctionEventCallBack funtion) { winData.eventCallBack = funtion; }

	protected:

		virtual void Init(const WindowOptions& options) = 0;
		
	protected:

		bool isCreateWindow;
		struct WindowData
		{
			unsigned int Height = 0;
			unsigned int Width = 0;
			String Title = "";

			FunctionEventCallBack eventCallBack;

		} winData;

	};
}