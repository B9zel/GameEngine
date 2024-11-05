#pragma once
#include <Events/include/Event.h>
#include <Core/includes/Dispatcher.h>
#include <Math/includes//Vector.h>



namespace CoreEngine
{

	class InputDevice
	{
	public:

		InputDevice() = default;

	public:

		static bool GetIsKeyPressed(uint32 key);
		static bool GetIsButtonPressed(uint32 button);
		static DVector2 GetMousePos();

		template<class Class>
		void AddBindEvent(MethodPtr<Class, void(Event&)>& ev)
		{
			m_dispatch.AddBind(ev);
		}
		
		template<class Class>
		void AddBindEvent(void(Class::* method)(Event&), Class* classOfMethod)
		{
			m_dispatch.AddBind(method, classOfMethod);
		}

		template<class Class>
		void RemoveEvent(void(Class::* method)(Event&), Class* classOfMethod)
		{
			m_dispatch.Remove(method, classOfMethod);
		}
		template<class Class>
		void RemoveEvent(MethodPtr<Class, void(Event&)>& ev)
		{
			m_dispatch.Remove(ev);
		}

		void InviteEvent(Event& event);

	private:

		Dispatcher<Event&> m_dispatch;
	};


	
}