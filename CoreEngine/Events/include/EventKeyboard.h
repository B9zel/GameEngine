#pragma once
#include "Event.h"


namespace CoreEngine
{
	class EventKeyboardPressed : public Event
	{
	public:
		EventKeyboardPressed() = default;
		EventKeyboardPressed(const int32 key) : m_Key{ key } {}

		int32 GetKey() const { return m_Key; }
		void SetKey(const int32 key) { m_Key = key; }

		CREATE_EVENT_CLASS(KEY_PRESSED)
	private:
		int32 m_Key;

	};

	class EventKeyboardReleased : public Event
	{
	public:
		EventKeyboardReleased() = default;
		EventKeyboardReleased(const int32 key) : m_Key{ key } {}

		int32 GetKey() const { return m_Key; }
		void SetKey(const int32 key) { m_Key = key; }

		CREATE_EVENT_CLASS(KEY_RELEASED)
	private:
		int32 m_Key;
	};


	class EventKeyboardRepeat : public Event
	{
	public:

		EventKeyboardRepeat() = default;
		EventKeyboardRepeat(const int32 key) : m_Key{ key } {}

		int32 GetKey() const { return m_Key; }
		void SetKey(const int32 key) { m_Key = key; }

		CREATE_EVENT_CLASS(KEY_REPEAT)
	private:
		int32 m_Key;
	};
}