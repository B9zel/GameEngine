#pragma once
#include <ReflectionSystem/Include/BaseField.h>




namespace CoreEngine
{
	namespace Reflection
	{
		enum class EStructFieldParams : uint8
		{
			NONE = 0
		};

		struct StructField : public ConstructionField
		{
			EStructFieldParams Params;
		};
	}
}