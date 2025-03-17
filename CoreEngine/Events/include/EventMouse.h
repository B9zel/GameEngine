#pragma once
#include "Event.h"


namespace CoreEngine
{
	class EventMouseButtonPressed : public Event
	{
	public:

		EventMouseButtonPressed() = default;

		EventMouseButtonPressed(const int32 button) : m_Button{ button } {}

		int32 GetButton() const { return m_Button; }
		void SetButton(const int32 button) { m_Button = button; }

		CREATE_EVENT_CLASS(MOUSE_BUTTON_PRESSED)
	private:
		int32 m_Button;
	};

	class EventMouseButtonReleased : public Event
	{
	public:

		EventMouseButtonReleased() = default;

		EventMouseButtonReleased(const int32 button) : m_Button{ button } {}

		int32 GetButton() const { return m_Button; }
		void SetButton(const int32 button) { m_Button = button; }

		CREATE_EVENT_CLASS(MOUSE_BUTTON_RELEASED)
	private:
		int32 m_Button;
	};

	class EventMouseMotion : public Event
	{
	public:

		EventMouseMotion() = default;

		EventMouseMotion(const double xpos, const double ypos) : m_PosX{ xpos }, m_PosY{ ypos } {}

		double GetPosX() const { return m_PosX; }
		double GetPosY() const { return m_PosY; }
		//int32 GetEvent() const { return m_EventID; }

		void SetPosX(const double xpos) { m_PosX = xpos; }
		void SetPosY(const double ypos) { m_PosY = ypos; }
		//void SetEvent(const int32 idEvent) { m_EventID = idEvent; }

		CREATE_EVENT_CLASS(MOUSE_MOVE)
	private:
		double m_PosX, m_PosY;
	};


	class EventMouseScroll : public Event
	{
	public:

		EventMouseScroll() = default;

		EventMouseScroll(const double xOffset, const double yOffset) : m_OffsetX{ xOffset }, m_OffsetY{ yOffset } {}

		double GetOffsetX() const { return m_OffsetX; }
		double GetOffsetY() const { return m_OffsetY; }

		void SetOffsetX(const double xOffset) { m_OffsetX = xOffset; }
		void SetOffsetY(const double yOffset) { m_OffsetY = yOffset; }

		CREATE_EVENT_CLASS(MOUSE_SCROLL)
	private:
		double m_OffsetX, m_OffsetY;
	};
}