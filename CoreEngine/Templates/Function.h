#pragma once
#include <utility>
#include "Core/includes/Base.h"



namespace CoreEngine
{
	template<class TReturn, class... Args>
	class Function {};

	/*template<class TClass>
	struct Bind
	{
		template<class Class, class TReturn, class... Args>
		Bind(TReturn(Class::*method)(Args...), Class* instance) : method{ reinterpret_cast<void*>(&method)}, instance{instance} {}

		void* method;
		TClass* instance;
		
	};*/
	


	enum class ETypeFunction : uint8_t
	{
		FUNCTION,
		METHOD
	};

	template<class TReturn, class... TArgs>
	struct BaseFnPtr {};

	template<class TReturn,class... TArgs>
	struct BaseFnPtr<TReturn(TArgs...)>
	{
	public:
		using FnPtr = TReturn(*)(TArgs...);
	public:
	
		virtual ~BaseFnPtr() = default;
	
		virtual TReturn Invoke(TArgs&&... param) = 0;
	
		virtual bool operator==(const BaseFnPtr<TReturn(TArgs...)>& other) const = 0;
	
		virtual ETypeFunction GetType() const = 0;
	
	};


	template<class TReturn, class... Args>
	struct FunctionPtr {};

	template<class TReturn, class... Args>
	struct FunctionPtr<TReturn(Args...)> : public BaseFnPtr<TReturn(Args...)>
	{
	public:
		using FnPtr = TReturn(*)(Args...);
	public:

		FunctionPtr(FnPtr function) : pFn {function} {}

		virtual TReturn Invoke(Args&&... param) override
		{
			if (std::is_void_v<TReturn>)
				(void)(pFn(std::forward<Args>(param)...));
			else
				return (pFn(std::forward<Args>(param)...));
		}

	
		virtual ETypeFunction GetType() const override
		{
			return typeFn;
		}


		bool operator==(const BaseFnPtr<TReturn(Args...)>& other) const override
		{
			if (static_cast<uint8_t>(GetType()) != static_cast<uint8_t>(other.GetType())) return false;

			const FunctionPtr& pMethod = dynamic_cast<const FunctionPtr&>(other);
			if (pFn != pMethod.pFn) return false;

			return true;
		}

	public:
		
		FnPtr pFn;
		const ETypeFunction typeFn = ETypeFunction::FUNCTION;
	};




	template<class TClass, class TReturn, class... Args>
	struct MethodPtr {};

	template<class TClass,class TReturn, class... Args>
	struct MethodPtr<TClass, TReturn(Args...)> : public BaseFnPtr<TReturn(Args...)>
	{
	private:

		using FnPtr = TReturn(TClass::*)(Args...);

	public:

		MethodPtr(FnPtr method, TClass* classOfMethod) : pFn{ method }, pClass{ classOfMethod } {}

		virtual TReturn Invoke(Args&&... param) override
		{
			if (std::is_void_v<TReturn>)
				(pClass->*pFn)(std::forward<Args>(param)...);
			else
				return (pClass->*pFn)(std::forward<Args>(param)...);
			
		}
		

		virtual ETypeFunction GetType() const override
		{
			return typeFn;
		}

		bool operator==(const BaseFnPtr<TReturn(Args...)>& other) const override
		{
			if (this->GetType() != other.GetType()) { return false; }

			const MethodPtr& pMethod = dynamic_cast<const MethodPtr&>(other);
			if (pFn != pMethod.pFn || pClass != pMethod.pClass) return false;
			
			return true;
		}
	public:
		TClass* pClass;
		FnPtr pFn;
		const ETypeFunction typeFn = ETypeFunction::METHOD;
	};




	template<class TReturn, class... Args>
	class Function<TReturn(Args...)>
	{
	public:

		using BaseFunctionPtr = BaseFnPtr<TReturn(Args...)>;
		using SharedBaseFunctionPtr = SharedPtr<BaseFunctionPtr>;
		using FnPtr = TReturn(*)(Args...);
		
	public:

		Function() = default;

		
		Function(FnPtr function)
		{
			Assign(function);
		}
		template<class TClass>
		Function(TReturn(TClass::*method)(Args...), TClass* instanceClass)
		{
			Assign<TClass>(method, instanceClass);
		}

		Function(const Function& other)
		{
			m_Fn = other.m_Fn;
		}

		Function(Function&& other)
		{
			m_Fn = std::move(other.m_Fn);
		}


	
	public:


		void Assign(FnPtr function)
		{
			if (m_Fn)
			{	
				if (m_Fn->GetType() == ETypeFunction::FUNCTION)
				{
					::new(m_Fn.get()) FunctionPtr<TReturn(Args...)>(function);
					return;
				}
			}
			m_Fn = MakeSharedPtr<FunctionPtr<TReturn(Args...)>>(function);
		}

		template<class TClass>
		void Assign(TReturn(TClass::*method)(Args...), TClass* instanceClass)
		{
			if (m_Fn)
			{
				if (m_Fn->GetType() == ETypeFunction::METHOD)
				{
					::new(m_Fn.get()) MethodPtr<TClass,TReturn(Args...)>(method, instanceClass);
					return;
				}
			}
			m_Fn = MakeSharedPtr<MethodPtr<TClass, TReturn(Args...)>>(method, instanceClass);
		}


		Function& operator=(const Function& other)
		{
			m_Fn = other.m_Fn;

			return *this;
		}

		Function& operator=(Function&& other)
		{
			m_Fn = std::move(other.m_Fn);

			return *this;
		}

		explicit operator bool() const
		{
			return m_Fn.operator bool();
		}
		

		bool IsEmpty() const
		{
			return !this->operator bool();
		}

		SharedBaseFunctionPtr GetFunction() const
		{
			return m_Fn;
		}

		TReturn Invoke(Args&&... param) const
		{
		#ifdef DEVELOPMENT_DEBUG
			if (IsEmpty())
			{
				EG_LOG(CORE, ELevelLog::WARNING, "Function don't valid");
			}
		#endif // DEVELOPMENT_DEBUG

			return m_Fn->Invoke(std::forward<Args>(param)...);
		}

		TReturn operator()(Args&&... param) const
		{
			return Invoke(std::forward<Args>(param)...);
		}

		bool operator==(const Function& other)
		{
			return (*m_Fn) == (*other.m_Fn);
		}

		bool operator!=(const Function& other)
		{
			return !this->operator==(other);
		}

	private:
		
		SharedBaseFunctionPtr m_Fn;

		
	};
	//template<class Class, class TRet, class... TArgs>
	//using PtrMethod = typename Function<TRet(TArgs)>::Method<Class> ;
}