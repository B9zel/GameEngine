#pragma once
#include "Event.h"

namespace CoreEngine
{
	class EventCloseWindow : public Event
	{
	public:

		CREATE_EVENT_CLASS(WINDOW_CLOSE)
	
	};

	class EventFocuseWindow : public Event
	{
	public:

		EventFocuseWindow(bool isEnter) : m_IsEnter{ isEnter } {}

		bool GetIsEnter() const { return m_IsEnter; }
		void SetIsEnter(bool isEnter) { m_IsEnter = isEnter; }

		CREATE_EVENT_CLASS(WINDOW_FOCUSED)
	private:
		bool m_IsEnter;
		static const EEventType m_typeEvent = EEventType::WINDOW_FOCUSED;
	};


	class EventResizeWidnow : public Event
	{
	public:

		EventResizeWidnow(int width, int height) : m_Width{ width }, m_Height{ height } {}

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }

		CREATE_EVENT_CLASS(WINDOW_RESIZE)

	private:
		int m_Width, m_Height;
	};

}