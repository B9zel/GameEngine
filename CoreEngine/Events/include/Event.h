#pragma once
#include <Core/includes/Base.h>

namespace CoreEngine
{

	enum class EEventType : unsigned
	{
		KEY_PRESSED = 0,
		KEY_RELEASED,
		KEY_REPEAT,
		MOUSE_BUTTON_PRESSED,
		MOUSE_BUTTON_RELEASED,
		MOUSE_SCROLL,
		MOUSE_MOVE,
		WINDOW_RESIZE,
		WINDOW_FOCUSED,
		WINDOW_CLOSE,

		COUNT_ENUM
	};


	DECLARE_LOG_CATEGORY_EXTERN(EVENT);
	#define CREATE_EVENT_CLASS(name) static EEventType GetStaticType() { return EEventType::name; } \
									virtual EEventType GetEventType() const override { return GetStaticType(); }

	class Event
	{
	public:

		virtual EEventType GetEventType() const = 0;
	};

	
	class EventDispatch
	{
	public:
		template<class T>
		void AddEvent(const Function<void(Event&)>& fn)
		{
		#ifdef DEVELOPMENT_DEBUG
			if (static_cast<unsigned>(T::GetStaticType()) >= static_cast<unsigned>(EEventType::COUNT_ENUM))
			{
				EG_LOG(EVENT, ELevelLog::WARNING, "Out of range of events");
				return;
			}
		#endif // DEVELOPMENT_DEBUG

			m_Event[static_cast<int>(T::GetStaticType())] = fn;
		}

		template<class T>
		bool Dispatch(Event& fn)
		{
			if (!m_Event[static_cast<int>(T::GetStaticType())]) return false;
			
			if (T::GetStaticType() == fn.GetEventType())
			{
				m_Event[static_cast<int>(T::GetStaticType())](static_cast<T&>(fn));
				return true;
			}
			return false;
		}


	private:

		StaticArray<Function<void(Event&)>, static_cast<int>(EEventType::COUNT_ENUM)> m_Event;
	};
}

#include "EventApplication.h"
#include "EventKeyboard.h"
#include "EventMouse.h"
