#pragma once
#include <Core/includes/Base.h>
#include <iostream>


namespace CoreEngine
{
	template<class... Args>
	class MDispatcher;



	template<class... Args>
	class Dispatcher
	{
	public: 

		using TypeParam = void(Args...);
		using FunctionParam = Function<TypeParam>;

	public:

		Dispatcher()
		{
			m_MtDispatch.reserve(1);
		}

		template<class Class>
		void AddBind(void(Class::* method)(Args...), Class* classOfMethod)
		{
			m_MtDispatch.push_back(FunctionParam(method, classOfMethod));
		}

		
		void AddBind(void(*Func)(Args...))
		{
			m_MtDispatch.push_back(FunctionParam(Func));
		}
		
		template<class Class>
		void AddBind(MethodPtr<Class, TypeParam>& bind)
		{
			m_MtDispatch.push_back(bind);
		}

		template<class Class>
		bool Remove(void(Class::*method)(Args...), Class* classOfMethod)
		{
			if (m_MtDispatch.empty()) return false;

			MethodPtr<Class, TypeParam> removeFn(method, classOfMethod);
			for (DArray<FunctionParam>::iterator it = m_MtDispatch.begin(); it != m_MtDispatch.end(); ++it)
			{
				if (*(it->GetFunction()) == removeFn)
				{
					m_MtDispatch.erase(it);
					return true;
				}
			}

			EG_LOG(CORE, ELevelLog::WARNING, "There is no function");
			return false;
		}

		inline bool RemoveAll()
		{
			if (m_MtDispatch.empty())
				return false;

			m_MtDispatch.clear();
			return true;
		}

		inline void Call(Args&&... param)
		{
			if (m_MtDispatch.empty()) return;

			for (auto& el : m_MtDispatch)
			{
				el(std::forward<Args>(param)...);
			}
		}

		const DArray<FunctionParam>& GetDispatch()
		{
			return m_MtDispatch;
		}

	private:

		DArray<FunctionParam> m_MtDispatch;
	};

}