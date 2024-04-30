#pragma once
#include <Events/include/Event.h>
#include <Core/includes/Dispatcher.h>




namespace CoreEngine
{

	class Input
	{
	public:

		Input() = default;

	public:

		static bool GetIsKeyPressed(unsigned int key);
		static bool GetIsButtonPressed(unsigned int button);

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