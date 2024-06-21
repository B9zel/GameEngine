#pragma once
#include <Runtime/includes/Object.h>
#include <Core/includes/Base.h>
#include <Core/includes/Memory/GarbageCollector.h>


namespace CoreEngine
{
	namespace Runtime
	{
		class Object;
	}

	namespace GB
	{
		template<class T>
		class GBNotify
		{
		public:

			GBNotify() = default;

			GBNotify(T val, Function<void(Runtime::Object*, Runtime::Object*)>& method);
			
			T operator=(T value);
			//T operator=(T&& value);


		private:

			T m_Property;
			Function<void(Runtime::Object*, Runtime::Object*)> m_Mehtod;

			friend GarbageCollector;
		};


		template<class T>
		inline GBNotify<T>::GBNotify(T val, Function<void(Runtime::Object*, Runtime::Object*)>& method)
		{
			m_Mehtod = method;
			if (std::is_pointer<decltype(val)>::value)
			{
				const bool isPointer = std::is_pointer<decltype(val)>::value && !std::is_pointer<std::remove_reference_t<decltype(*val)>>::value;
				if (isPointer)
				{
					m_Property = val;
				}
				CORE_ASSERT(!isPointer, "GBNotify can't take pointer to pointer");
				return;
			}
			CORE_ASSERT(true, "GBNotify can take only pointer");
		}

		template<class T>
		inline T GBNotify<T>::operator=(T value)
		{
			void* oldData = m_Property;
			m_Property = value;

			if (oldData != m_Property)
			{
				m_Mehtod.Invoke(std::move(oldData), m_Property);
			}

			return m_Property;
		}
		/*template<class T>
		inline T GBNotify<T>::operator=(T&& value)
		{




			return T();
		}*/
	}
}