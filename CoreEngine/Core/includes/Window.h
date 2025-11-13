#pragma once
#include <Core/includes/Base.h>
#include <Events/include/Event.h>
#include <Render/includes/GraphicsContext.h>


namespace CoreEngine
{
	struct WindowOptions
	{
		WindowOptions() {}
		WindowOptions(const String& title, uint32 width, uint32 height) : Title{ title }, Width{width}, Height{height} {}
		String Title = "";
		uint32 Width = 0;
		uint32 Height = 0;
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
		
		virtual uint32 GetWidth() const = 0;

		virtual uint32 GetHeight() const = 0;

		virtual String GetTitle() const = 0;

		virtual void* GetNativeWindow() const = 0;

		virtual bool GetIsCreateWindow() const;

		virtual Render::GraphicsContext* GetContext() const = 0;

		void SetEventBind(const FunctionEventCallBack function) { winData.eventCallBack = function; }

	protected:

		virtual void Init(const WindowOptions& options) = 0;
		
	protected:

		bool isCreateWindow;
		struct WindowData
		{
			uint32 Height = 0;
			uint32 Width = 0;
			String Title = "";
			
			FunctionEventCallBack eventCallBack;

		} winData;
	};
}