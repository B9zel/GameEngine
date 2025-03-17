#pragma once
#include <Events/include/Event.h>

namespace CoreEngine
{
	struct EventCell
	{
		EventCell()
		{
			EG_LOG(CORE, ELevelLog::INFO, "Create {0}", countCreate);
			countCreate++;
		}

		EventCell(EventCell&& Other)
		{
			IsFree = Other.IsFree;
			event = std::move(Other.event);

			Other.IsFree = true;
		}

		UniquePtr<Event> event = nullptr;
		bool IsFree = true;
		static int32 countCreate;
	};
	

	class EventPool
	{
	public:

		EventPool();

	public:

		void ConstructPool();
		Event* GetEventClass(const EEventType& eventType);
		void GiveEventClass(Event* event);

		UniquePtr<Event> CreateEventBuyType(const EEventType& eType);

	private:

		HashTableMap<EEventType, DArray<UniquePtr<EventCell>>> m_PoolEvents;
		uint32 spaceEvents;
	};
}