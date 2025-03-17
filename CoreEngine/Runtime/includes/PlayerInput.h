#pragma once
#include <Core/includes/Base.h>
#include <Events/include/Event.h>
#include <Events/include/EventPool.h>


namespace CoreEngine
{
	namespace Runtime
	{
		class PlayerController;


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

			Queue<Event*>* GetReverseQueueEvents();

			void ResetQueueEvents();

		private:

			void Register();
			void TakeEvent(Event& event);

			void OnRemoveElement(Event** event);

		private:

			Queue<Event*> m_Events;
			EventPool m_eventPool;

			friend PlayerController;
		};
	}
}