#pragma once
#include <Core/includes/Base.h>
#include <Events/include/Event.h>
#include <Events/include/EventPool.h>

class PlayerController;

namespace CoreEngine
{
	namespace Runtime
	{
		struct EventBuffer
		{
			EventBuffer() = default;

			union
			{
				EventResizeWidnow eventResize;
				EventCloseWindow eventCloseWindow;
				EventFocusedWindow eventFucusWin;

				EventMouseScroll eventMouseScroll;
				EventMouseMotion eventMouseMotion;
				EventMouseButtonPressed eventMousePressed;
				EventMouseButtonReleased eventMouseReleased;

				EventKeyboardPressed eventKeyPressed;
				EventKeyboardReleased eventKeyReleased;
				EventKeyboardRepeat eventKeyRepeat;

			} storeEvent;

			EEventType TypeEvent;
		};

		/*class EventQueue
		{
		public:

			EventQueue() = default;

		public:

			void PushBack(Event& event)
			{

			}

		private:

			DArray<Event> m_QueueEvents;
			uint32 Front;
			uint32 Back;
		};*/

		class PlayerInput
		{
		public:

			PlayerInput() = default;
			~PlayerInput();

		public:

			Queue<CoreEngine::Event*>* GetReverseQueueEvents();

			void ResetQueueEvents();

		private:

			void Register();
			void TakeEvent(CoreEngine::Event& event);

			void OnRemoveElement(CoreEngine::Event** event);

		private:

			Queue<CoreEngine::Event*> m_Events;
			EventPool m_eventPool;

			friend PlayerController;
		};
	} // namespace Runtime
} // namespace CoreEngine
