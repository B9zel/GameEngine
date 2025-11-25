#pragma once
#include <utility>
#include <iostream>
#include <Core/includes/Base.h>



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
	
	virtual TReturn Invoke(TArgs&&... param) const = 0;
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

	FunctionPtr(const FunctionPtr& Other)
	{
		pFn = Other.pFn;
	}
	FunctionPtr(FunctionPtr&& Other)
	{
		pFn = Other.pFn;
		Other.pFn = nullptr;
	}

	FunctionPtr& operator=(const FunctionPtr& Other)
	{
		pFn = Other.pFn;

		return *this;
	}

	FunctionPtr& operator=(FunctionPtr&& Other)
	{
		pFn = Other.pFn;
		Other.pFn = nullptr;

		return *this;
	}
	
public:

	virtual TReturn Invoke(Args&&... param) const override
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
		if (static_cast<uint8>(GetType()) != static_cast<uint8>(other.GetType())) return false;

		const FunctionPtr& pMethod = dynamic_cast<const FunctionPtr&>(other);
		if (pFn != pMethod.pFn) return false;

		return true;
	}

	FunctionPtr& operator=(FnPtr NewFunction)
	{
		pFn = NewFunction;
		return *this;
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

	MethodPtr(TClass* classOfMethod, FnPtr method) : pFn{ method }, pClass{ classOfMethod } {}

	virtual TReturn Invoke(Args&&... param) const override
	{
		if (!pClass || !pFn)
		{
			return TReturn();
		}
		
		return (pClass->*pFn)(std::forward<Args>(param)...);
	}
	
	void Assign(TClass* classOfMethod, FnPtr method)
	{
		pClass = classOfMethod;
		pFn = method;
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

	MethodPtr& operator=(const MethodPtr& Other)
	{
		pClass = Other.pClass;
		pFn = Other.pFn;

		return *this;
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
				::new(m_Fn.get()) MethodPtr<TClass,TReturn(Args...)>(instanceClass, method);
				return;
			}
		}
		m_Fn = MakeSharedPtr<MethodPtr<TClass, TReturn(Args...)>>(instanceClass, method);
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
