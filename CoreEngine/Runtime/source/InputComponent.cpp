#include <Runtime/includes/InputComponent.h>
#include <Core/includes/InputDevice.h>
#include <Runtime/includes/Actor.h>

DECLARE_LOG_CATEGORY_EXTERN(INPUT_COMPONENT_LOG);

InputComponent::InputComponent(const CoreEngine::InitializeObject& Object) : ActorComponent(Object)
{
	PressedKey = INT_MIN;
}

bool InputComponent::ProcessUpdateAction(CoreEngine::Event* event)
{
	for (auto& i : m_CallbackInputAction)
	{
		switch (event->GetEventType())
		{
		case CoreEngine::EEventType::KEY_PRESSED:
		{
			auto* Event = dynamic_cast<CoreEngine::EventKeyboardPressed*>(event);
			if (m_CallbackInputAction.count(Event->GetKey()))
			{
				for (auto& Input : m_CallbackInputAction[Event->GetKey()])
				{
					if (Input.Action == CoreEngine::EActionType::PRESSED)
					{
						Input.Callback.Execute();
						return true;
					}
				}
			}

			break;
		}
		case CoreEngine::EEventType::KEY_RELEASED:
		{
			auto* Event = dynamic_cast<CoreEngine::EventKeyboardReleased*>(event);
			if (m_CallbackInputAction.count(Event->GetKey()))
			{
				for (auto& Input : m_CallbackInputAction[Event->GetKey()])
				{
					if (Input.Action == CoreEngine::EActionType::RELEASED)
					{
						Input.Callback.Execute();
						return true;
					}
				}
			}

			break;
		}
		case CoreEngine::EEventType::MOUSE_BUTTON_PRESSED:
		{
			auto* Event = dynamic_cast<CoreEngine::EventMouseButtonPressed*>(event);
			if (m_CallbackInputAction.count(Event->GetButton()))
			{
				for (auto& Input : m_CallbackInputAction[Event->GetButton()])
				{
					if (Input.Action == CoreEngine::EActionType::PRESSED)
					{
						Input.Callback.Execute();
						return true;
					}
				}
			}

			break;
		}
		case CoreEngine::EEventType::MOUSE_BUTTON_RELEASED:
		{
			auto* Event = dynamic_cast<CoreEngine::EventMouseButtonReleased*>(event);
			if (m_CallbackInputAction.count(Event->GetButton()))
			{
				for (auto& Input : m_CallbackInputAction[Event->GetButton()])
				{
					if (Input.Action == CoreEngine::EActionType::RELEASED)
					{
						Input.Callback.Execute();
						return true;
					}
				}
			}

			break;
		}
		}
	}
	if (event->GetEventType() == CoreEngine::EEventType::MOUSE_MOVE)
	{
		auto* Event = dynamic_cast<CoreEngine::EventMouseMotion*>(event);
		for (auto& Bind : m_CallbackInputMotion)
		{
			Bind.Callback.Execute(Event->GetPosX(), Event->GetPosY());
		}
	}
	return false;
}

bool InputComponent::ProcessUpdateAxis(CoreEngine::Event* event)
{

	switch (event->GetEventType())
	{
	case CoreEngine::EEventType::KEY_PRESSED:
	{
		auto* Event = dynamic_cast<CoreEngine::EventKeyboardPressed*>(event);
		if (m_CallbackInputAxis.count(Event->GetKey()))
		{
			BindInputAxisInfo& Axis = m_CallbackInputAxis[Event->GetKey()];
			Axis.isHold = true;
			return true;
		}

		break;
	}
	case CoreEngine::EEventType::KEY_RELEASED:
	{
		auto* Event = dynamic_cast<CoreEngine::EventKeyboardReleased*>(event);
		if (m_CallbackInputAxis.count(Event->GetKey()))
		{
			BindInputAxisInfo& Axis = m_CallbackInputAxis[Event->GetKey()];
			Axis.isHold = false;
			return true;
		}

		break;
	}
	case CoreEngine::EEventType::MOUSE_BUTTON_PRESSED:
	{
		auto* Event = dynamic_cast<CoreEngine::EventMouseButtonPressed*>(event);
		if (m_CallbackInputAxis.count(Event->GetButton()))
		{
			BindInputAxisInfo& Axis = m_CallbackInputAxis[Event->GetButton()];
			Axis.isHold = true;
			return true;
		}

		break;
	}
	case CoreEngine::EEventType::MOUSE_BUTTON_RELEASED:
	{
		auto* Event = dynamic_cast<CoreEngine::EventMouseButtonReleased*>(event);
		if (m_CallbackInputAxis.count(Event->GetButton()))
		{
			BindInputAxisInfo& Axis = m_CallbackInputAxis[Event->GetButton()];

			Axis.isHold = false;
			return true;
		}

		break;
	}
	}
	return false;
}

bool InputComponent::ExecuteKeyCallback(const int32 Key, const CoreEngine::EActionType& Action)
{
	if (!m_CallbackInputAction.count(Key)) return false;

	for (auto& Input : m_CallbackInputAction[Key])
	{
		if (Input.Action == Action)
		{
			Input.Callback.Execute();
		}
	}

	return true;
}

bool InputComponent::ChangeAxisAction(const int32 Key, const CoreEngine::EActionType& NewAction)
{
	if (!m_CallbackInputAxis.count(Key)) return false;

	BindInputAxisInfo& Axis = m_CallbackInputAxis[Key];
	Axis.Callback.Execute(Axis.ValueInput);

	return true;
}

void InputComponent::ExecuteEvent(CoreEngine::Event& event)
{
	// OnInputEvent(event);
}
void InputComponent::ExecuteAxis()
{
	if (m_CallbackInputAxis.empty()) return;

	for (auto& el : m_CallbackInputAxis)
	{
		BindInputAxisInfo& Axis = el.second;

		if (Axis.isHold)
		{
			Axis.Callback.Execute(Axis.ValueInput);
		}
		else
		{
			Axis.Callback.Execute(0.0f);
		}
	}
}
const HashTableMap<int, DArray<BindInputActionInfo>>& InputComponent::GetActionBinds() const
{
	return m_CallbackInputAction;
}
const HashTableMap<int, BindInputAxisInfo>& InputComponent::GetAxisBinds() const
{
	return m_CallbackInputAxis;
}
