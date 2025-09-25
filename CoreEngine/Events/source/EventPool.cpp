#include <Events/include/EventPool.h>
#include <Core/includes/Memory/Allocator.h>



namespace CoreEngine
{
	int32 EventCell::countCreate = 0;

	CoreEngine::EventPool::EventPool()
	{
		spaceEvents = 10;
	}

	void CoreEngine::EventPool::ConstructPool()
	{
		m_PoolEvents.reserve(static_cast<uint8>(EEventType::COUNT_ENUM));
		m_PoolEvents[EEventType::KEY_PRESSED].resize(spaceEvents);
		m_PoolEvents[EEventType::KEY_RELEASED].resize(spaceEvents);
		m_PoolEvents[EEventType::KEY_REPEAT].resize(spaceEvents);
		m_PoolEvents[EEventType::MOUSE_BUTTON_PRESSED].resize(spaceEvents);
		m_PoolEvents[EEventType::MOUSE_BUTTON_RELEASED].resize(spaceEvents);
		m_PoolEvents[EEventType::MOUSE_MOVE].resize(spaceEvents);
		m_PoolEvents[EEventType::MOUSE_SCROLL].resize(spaceEvents);
		m_PoolEvents[EEventType::WINDOW_CLOSE].resize(spaceEvents);
		m_PoolEvents[EEventType::WINDOW_FOCUSED].resize(spaceEvents);
		m_PoolEvents[EEventType::WINDOW_RESIZE].resize(spaceEvents);


		for (auto& i : m_PoolEvents)
		{
			for (uint64 j = 0; j < i.second.size(); j++)
			{
				UniquePtr< EventCell> newCells = MakeUniquePtr<EventCell>();
				newCells->event = CreateEventByType(i.first);
				i.second[j] = std::move(newCells);
			}

		}
	}

	Event* CoreEngine::EventPool::GetEventClass(const EEventType& eventType)
	{
		for (auto& i : m_PoolEvents[eventType])
		{
			if (i->IsFree)
			{
				i->IsFree = false;
				return i->event.get();
			}
		}
		return nullptr;
	}

	void CoreEngine::EventPool::GiveEventClass(Event* event)
	{
		for (auto& i : m_PoolEvents[event->GetEventType()])
		{
			if (i->event.get() == event)
			{
				i->IsFree = true;
			}
		}
	}

	UniquePtr<Event> EventPool::CreateEventByType(const EEventType& eType)
	{
		switch (eType)
		{
		case EEventType::KEY_PRESSED: return MakeUniquePtr<EventKeyboardPressed>();
		case EEventType::KEY_RELEASED: return MakeUniquePtr<EventKeyboardReleased>();
		case EEventType::KEY_REPEAT: return MakeUniquePtr<EventKeyboardRepeat>();
		case EEventType::MOUSE_BUTTON_PRESSED: return MakeUniquePtr<EventMouseButtonPressed>();
		case EEventType::MOUSE_BUTTON_RELEASED: return MakeUniquePtr<EventMouseButtonReleased>();
		case EEventType::MOUSE_MOVE: return MakeUniquePtr<EventMouseMotion>();
		case EEventType::MOUSE_SCROLL: return MakeUniquePtr<EventMouseScroll>();
		case EEventType::WINDOW_CLOSE: return MakeUniquePtr<EventCloseWindow>();
		case EEventType::WINDOW_FOCUSED: return MakeUniquePtr<EventFocusedWindow>();
		case EEventType::WINDOW_RESIZE: return MakeUniquePtr<EventResizeWidnow>();
		default:
			break;
		}
	}
}