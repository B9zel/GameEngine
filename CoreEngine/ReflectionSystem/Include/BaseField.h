#pragma once
#include <Core/includes/Base.h>
//#include <ReflectionSystem/Include/PropertyField.h>


namespace CoreEngine
{
	namespace Reflection
	{


		struct BaseField
		{
			virtual ~BaseField() noexcept = default;

		public:

			BaseField() {
			}

			bool operator==(const BaseField& Other) const
			{
				return Other.Name == Name;
			}

			bool operator!=(const BaseField& Other) const
			{
				return !this->operator==(Other);
			}


		public:

			String Name;
		};


	}
}