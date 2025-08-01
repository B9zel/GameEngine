#pragma once
#include <Core/includes/Base.h>
//#include <ReflectionSystem/Include/PropertyField.h>


namespace CoreEngine
{
	namespace Reflection
	{
		struct BaseField
		{
			virtual ~BaseField() {};

		public:

			String Name;
		};


	}
}