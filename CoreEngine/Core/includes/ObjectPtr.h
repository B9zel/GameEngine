#pragma once

#include <Core/includes/Base.h>
#include <Core/includes/Memory/GarbageCollector.h>
#include <Core/includes/MemoryManager.h>



namespace CoreEngine
{
	class Application;
	namespace Runtime
	{
		class Object;
	}

	namespace GB
	{
		class GarbageCollector;
	}

	template<class T>
	class ObjectPtr
	{
	public:

		~ObjectPtr();


		ObjectPtr();
		ObjectPtr(T* value);
		ObjectPtr(ObjectPtr&& Other) noexcept;

		T* operator=(T* value);
		ObjectPtr& operator=(const ObjectPtr& other);
		ObjectPtr& operator=(ObjectPtr&& other) noexcept;

		bool IsValid() const;


		operator bool() const
		{
			return IsValid();
		}

		T* operator->() const
		{
			return m_Property;
		}

		T& operator*() const
		{
			return *m_Property;
		}

		T* Get() const
		{
			return m_Property;
		}

	private:

		T* m_Property;
		Function<void(Runtime::Object*, Runtime::Object*)> m_Method;

		friend GB::GarbageCollector;
	};


	template<class T>
	inline ObjectPtr<T>::~ObjectPtr()
	{
		if (m_Property)
		{
			Engine::Get()->GetMemoryManager()->GetGarbageCollector()->RemoveReference(reinterpret_cast<Runtime::Object*>(m_Property));
		}
	}

	template<class T>
	inline ObjectPtr<T>::ObjectPtr() : m_Property{ nullptr }
	{
		m_Property = nullptr;
		Engine::Get()->GetMemoryManager()->GetGarbageCollector()->AddProperty(this);
	}


	template<class T>
	inline ObjectPtr<T>::ObjectPtr(T* value) : ObjectPtr()
	{
		m_Property = value;

		m_Method.Invoke(std::move(nullptr), std::move(m_Property));
	}

	template<class T>
	inline ObjectPtr<T>::ObjectPtr(ObjectPtr&& Other) noexcept : ObjectPtr()
	{
		T* oldData = m_Property;
		m_Property = Other.m_Property;

		Other.m_Property = nullptr;

		//m_Method.Invoke(std::move(oldData), std::move(m_Property));
		//Other.m_Method.Invoke(m_Property, nullptr);
	}

	template<class T>
	inline T* ObjectPtr<T>::operator=(T* value)
	{
		T* oldData = m_Property;
		m_Property = value;

		if (oldData != m_Property)
		{
			m_Method.Invoke(std::move(oldData), std::move(m_Property));
		}

		return m_Property;
	}

	template<class T>
	ObjectPtr<T>& ObjectPtr<T>::operator=(const ObjectPtr& other)
	{
		T* oldPtr = m_Property;
		m_Property = Other.m_Property;

		if (oldPtr != m_Property)
		{
			m_Method.Invoke(std::move(oldPtr), std::move(m_Property));
		}

		return *this;
	}

	template<class T>
	ObjectPtr<T>& ObjectPtr<T>::operator=(ObjectPtr&& other) noexcept
	{
		T* oldData = m_Property;

		m_Property = Other.m_Property;
		Other.m_Property = nullptr;

		///	m_Method.Invoke(std::move(oldData), std::move(m_Property));
		//	Other.m_Method.Invoke(m_Property, nullptr);

		return *this;
	}

	template<class T>
	inline bool ObjectPtr<T>::IsValid() const
	{
		return m_Property != nullptr;
	}

}