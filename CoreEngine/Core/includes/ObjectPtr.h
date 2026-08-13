#pragma once

#include <Core/includes/Base.h>
#include <Core/includes/Memory/GarbageCollector.h>
#include <Runtime/CoreObject/Include/Object.h>

namespace CoreEngine
{
	class Application;
	namespace GB
	{
		class GarbageCollector;
	}

	template <class T> class ObjectPtr
	{
	public:

		~ObjectPtr();

		ObjectPtr() noexcept = default;
		ObjectPtr(T* value) noexcept;
		ObjectPtr(const ObjectPtr& other) noexcept;
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

		void AddReference() noexcept;
		void RemoveReference() noexcept;

		T* m_Property{nullptr};

		friend GB::GarbageCollector;
	};

	template <class T> inline ObjectPtr<T>::~ObjectPtr()
	{
		RemoveReference();
	}

	template <class T> inline ObjectPtr<T>::ObjectPtr(T* value) noexcept : m_Property(value)
	{
		AddReference();
	}

	template <class T> inline ObjectPtr<T>::ObjectPtr(const ObjectPtr& other) noexcept : m_Property(other.m_Property)
	{
		AddReference();
	}

	template <class T> inline ObjectPtr<T>::ObjectPtr(ObjectPtr&& Other) noexcept : m_Property(Other.m_Property)
	{
		Other.m_Property = nullptr;
	}

	template <class T> inline T* ObjectPtr<T>::operator=(T* value)
	{
		if (m_Property != value)
		{
			RemoveReference();
			m_Property = value;
			AddReference();
		}

		return m_Property;
	}

	template <class T> ObjectPtr<T>& ObjectPtr<T>::operator=(const ObjectPtr& other)
	{
		operator=(other.m_Property);
		return *this;
	}

	template <class T> ObjectPtr<T>& ObjectPtr<T>::operator=(ObjectPtr&& other) noexcept
	{
		if (this == &other) return *this;

		RemoveReference();
		m_Property = other.m_Property;
		other.m_Property = nullptr;
		return *this;
	}

	template <class T> inline bool ObjectPtr<T>::IsValid() const
	{
		return m_Property != nullptr;
	}

	template <class T> inline void ObjectPtr<T>::AddReference() noexcept
	{
		if (!m_Property) return;

		if (auto* collector = GB::GarbageCollector::GetGBInstance())
		{
			collector->AddReference(static_cast<Object*>(m_Property));
		}
	}

	template <class T> inline void ObjectPtr<T>::RemoveReference() noexcept
	{
		if (!m_Property) return;

		if (auto* collector = GB::GarbageCollector::GetGBInstance())
		{
			collector->RemoveReference(static_cast<Object*>(m_Property));
		}
	}

} // namespace CoreEngine
