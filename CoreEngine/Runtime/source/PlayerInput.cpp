#include <Runtime/includes/PlayerInput.h>
#include <Core/includes/InputDevice.h>
#include <Core/includes/Engine.h>
#include <Runtime/includes/PlayerController.h>
#include <Events/include/Event.h>


#define RESERVE_SPACE_EVENTS 5


namespace CoreEngine
{
	namespace Runtime
	{
		PlayerInput::~PlayerInput()
		{

		}
		Queue<Event*>* PlayerInput::GetReverseQueueEvents()
		{
			return &m_Events;
		}

		void PlayerInput::ResetQueueEvents()
		{
			while (!m_Events.IsEmpty())
			{
				m_eventPool.GiveEventClass(m_Events.Back());
				m_Events.PopBack();
			}
		}

		void PlayerInput::Register()
		{
			Engine::Get()->GetInputDevice()->AddBindEvent(&PlayerInput::TakeEvent, this);
			m_eventPool.ConstructPool();
			m_Events.RemoveElement.AddBind(&PlayerInput::OnRemoveElement, this);
		}

		void PlayerInput::TakeEvent(Event& event)
		{

			switch (event.GetEventType())
			{
			case EEventType::KEY_PRESSED:
			{
				auto* newEvent = dynamic_cast<EventKeyboardPressed*>(m_eventPool.GetEventClass(event.GetEventType()));
				newEvent->SetKey(dynamic_cast<EventKeyboardPressed&>(event).GetKey());
				m_Events.PushBack(newEvent);

				break;
			}
			case EEventType::KEY_RELEASED:
			{
				auto* newEvent = dynamic_cast<EventKeyboardReleased*>(m_eventPool.GetEventClass(event.GetEventType()));
				newEvent->SetKey(dynamic_cast<EventKeyboardReleased&>(event).GetKey());
				m_Events.PushBack(newEvent);

				break;
			}
			case EEventType::MOUSE_BUTTON_PRESSED:
			{
				auto* newEvent = dynamic_cast<EventMouseButtonPressed*>(m_eventPool.GetEventClass(event.GetEventType()));
				newEvent->SetButton(dynamic_cast<EventMouseButtonPressed&>(event).GetButton());
				m_Events.PushBack(newEvent);

				break;
			}
			case EEventType::MOUSE_BUTTON_RELEASED:
			{
				auto* newEvent = dynamic_cast<EventMouseButtonReleased*>(m_eventPool.GetEventClass(event.GetEventType()));
				newEvent->SetButton(dynamic_cast<EventMouseButtonReleased&>(event).GetButton());
				m_Events.PushBack(newEvent);

				break;
			}
			case EEventType::MOUSE_MOVE:
			{
				auto* newEvent = dynamic_cast<EventMouseMotion*>(m_eventPool.GetEventClass(event.GetEventType()));
				auto& TakeEvent = dynamic_cast<EventMouseMotion&>(event);
				newEvent->SetPosX(TakeEvent.GetPosX());
				newEvent->SetPosY(TakeEvent.GetPosY());
				m_Events.PushBack(newEvent);

				break;
			}
			}
		}

		void PlayerInput::OnRemoveElement(Event** event)
		{
			m_eventPool.GiveEventClass(*event);
		}

	}
}