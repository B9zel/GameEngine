#pragma once

#include <ReflectionSystem/Include/BaseField.h>



namespace CoreEngine
{
	namespace Reflection
	{
		struct ConstructionField;

		enum class EPropertyFieldParams : uint8
		{
			NONE = 0
		};

		enum class EPrimitiveTypes : uint8
		{
			NONE = 0,

			INT8,
			INT16,
			INT32,
			INT64,
			UINT8,
			UINT16,
			UINT32,
			UINT64,
			FLOAT_SINGLE,
			FLOAT_DOUBLE,
			STRING,
			BOOL,
			CHAR
		};


		struct TypePropertyField
		{
			~TypePropertyField() noexcept {}

			TypePropertyField(const TypePropertyField& Other) = default;

		public:
			bool IsComplexType;
			EPrimitiveTypes Primitive;
			union
			{
				String NameType;
				ConstructionField* ComplexType = nullptr;
			};
		};

		struct PropertyField : public BaseField
		{

			virtual ~PropertyField() noexcept {
			}

		public:

			uint32 Size;
			uint32 Offset;
			bool IsPointer;
			EPropertyFieldParams Params;
			TypePropertyField TypeProperty;


			template<class TypeProperty>
			TypeProperty* GetSourcePropertyByName(void* InstanceClass)
			{
				return reinterpret_cast<TypeProperty*>(InstanceClass + Offset);
			}
		};

	}
}