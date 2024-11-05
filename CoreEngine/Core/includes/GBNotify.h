#pragma once
#include <Core/includes/Engine.h>
//#include <Runtime/CoreObject/Include/Object.h>
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

		template<class T>
		class GBNotify
		{
		public:

			GBNotify();
			GBNotify(T value);

			T operator=(T value);
			T operator=(GBNotify&& value);
			GBNotify& operator=(const GBNotify& other);


			operator bool()
			{
				return m_Mehtod != nullptr;
			}

			T operator->()
			{
				return m_Property
			}

			std::add_lvalue_reference_t<T>& operator*()
			{
				return *m_Property;
			}

		private:

			T m_Property;
			Function<void(Runtime::Object*, Runtime::Object*)> m_Mehtod;

			friend GarbageCollector;
		};


		template<class T>
		inline GBNotify<T>::GBNotify()
		{
			if (std::is_pointer<T>::value)
			{
				T property;
				const bool isPointer = std::is_pointer<decltype(property)>::value && !std::is_pointer<std::remove_reference_t<decltype(*property)>>::value;
				if (isPointer)
				{
					m_Property = nullptr;
					Engine::Get()->GetMamoryManager()->GetGarbageCollector()->AddProperty(this);
				}
				CORE_ASSERT(!isPointer, "GBNotify can't take pointer to pointer");
				return;
			}
			CORE_ASSERT(true, "GBNotify can take only pointer");
		}

		template<class T>
		inline GBNotify<T>::GBNotify(T value) : GBNotify()
		{
			m_Property = value;
		}

		template<class T>
		inline T GBNotify<T>::operator=(T value)
		{
			void* oldData = m_Property;
			m_Property = value;

			if (oldData != m_Property)
			{
				m_Mehtod.Invoke(static_cast<T>(std::move(oldData)), std::move(m_Property));
			}

			return m_Property;
		}

		template<class T>
		inline T GBNotify<T>::operator=(GBNotify&& value)
		{
			this->operator=(std::forward(value));
			value.operator=(nullptr);

			return m_Property;
		}

		template<class T>
		GBNotify<T>& GBNotify<T>::operator=(const GBNotify& other)
		{
			this->operator=(other.m_Property);

			return *this;
		}
	}
}