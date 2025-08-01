#pragma once
#include <ReflectionSystem/Include/BaseField.h>
#include <ReflectionSystem/Include/ConstructionField.h>





namespace CoreEngine
{
	namespace Reflection
	{
		enum class EClassFieldParams : uint8
		{
			NONE = 0
		};

		struct ClassField : public ConstructionField
		{
			EClassFieldParams Params;
		};
	}
}