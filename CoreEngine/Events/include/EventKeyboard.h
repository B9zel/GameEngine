#pragma once
#include "Event.h"


namespace CoreEngine
{
	class EventKeyboardPressed : public Event
	{
	public:
		EventKeyboardPressed(const int key) : m_Key{ key } {}

		int GetKey() const { return m_Key; }
		void SetKey(const int key) { m_Key = key; }

		CREATE_EVENT_CLASS(KEY_PRESSED)
	private:
		int m_Key;

	};

	class EventKeyboardReleased : public Event
	{
	public:
		EventKeyboardReleased(const int key) : m_Key{ key } {}

		int GetKey() const { return m_Key; }
		void SetKey(const int key) { m_Key = key; }

		CREATE_EVENT_CLASS(KEY_RELEASED)
	private:
		int m_Key;
	};


	class EventKeyboardRepeat : public Event
	{
	public:

		EventKeyboardRepeat(const int key) : m_Key{ key } {}

		int GetKey() const { return m_Key; }
		void SetKey(const int key) { m_Key = key; }

		CREATE_EVENT_CLASS(KEY_REPEAT)
	private:
		int m_Key;
	};
}