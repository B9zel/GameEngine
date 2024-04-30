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

		template<class Class>
		void AddBind(void(Class::* method)(Args...), Class* classOfMethod)
		{
			m_MtDispath.push_back(FunctionParam(method, classOfMethod));
		}
		
		template<class Class>
		void AddBind(MethodPtr<Class, TypeParam>& bind)
		{
			m_MtDispath.push_back(bind);
		}

		template<class Class>
		bool Remove(void(Class::*method)(Args...), Class* classOfMethod)
		{
			if (m_MtDispath.empty()) return false;

			MethodPtr<Class, TypeParam> removeFn(method, classOfMethod);
			for (DArray<FunctionParam>::iterator it = m_MtDispath.begin(); it != m_MtDispath.end(); ++it)
			{
				if (*(it->GetFunction()) == removeFn)
				{
					m_MtDispath.erase(it);
					return true;
				}
			}

			EG_LOG(CORE, ELevelLog::WARNING, "There is no function");
			return false;
		}

		inline bool RemoveAll()
		{
			if (m_MtDispath.empty())
				return false;

			m_MtDispath.clear();
			return true;
		}

		inline void Call(Args&&... param)
		{
			for (auto& el : m_MtDispath)
			{
				el(std::forward<Args>(param)...);
			}
		}

		const DArray<FunctionParam>& GetDispatch()
		{
			return m_MtDispath;
		}
	private:
		DArray<FunctionParam> m_MtDispath;
	};

}