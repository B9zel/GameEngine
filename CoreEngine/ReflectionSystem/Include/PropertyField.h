#pragma once
#include <Core/includes/Base.h>
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

		EPrimitiveTypes ConvertToPropertyEnumFromString(const StringView& NameType);


		struct TypePropertyField
		{

			TypePropertyField() = default;
			~TypePropertyField() noexcept {}
			TypePropertyField(const String& Type)
			{
				// Is pointer
				uint64 Pos = Type.find("*");
		
				NameType = Type;
				IsComplexType = Pos >= 0;
				Primitive = ConvertToPropertyEnumFromString(IsComplexType ? Type.substr(0, Pos) : Type);
			}

		public:

			TypePropertyField(const TypePropertyField& Other) = default;
			TypePropertyField( TypePropertyField&& Other) = default;
			TypePropertyField& operator=(TypePropertyField&&) = default;
			TypePropertyField& operator=(const TypePropertyField& Other)
			{
				IsComplexType = Other.IsComplexType;
				Primitive = Other.Primitive;
				if (IsComplexType)
				{
					ComplexType = Other.ComplexType;
				}
				else
				{
					NameType = Other.NameType;
				}
				return *this;
			}

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

			PropertyField() = default;
			PropertyField(const PropertyField& Other)
			{
				Size = Other.Size;
				Offset = Other.Offset;
				IsPointer = Other.IsPointer;
				Params = Other.Params;
				TypeProperty = Other.TypeProperty;
			}

			PropertyField& operator=(const PropertyField& Other)
			{
				Size = Other.Size;
				Offset = Other.Offset;
				IsPointer = Other.IsPointer;
				Params = Other.Params;
				TypeProperty = Other.TypeProperty;
				return *this;
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
				return reinterpret_cast<TypeProperty*>(reinterpret_cast<uint64>(InstanceClass) + Offset);
			}

			template<typename TypeProperty>
			void SetSourceProperty(void* InstanceClass, const TypeProperty NewValur)
			{
				TypeProperty* Variable = reinterpret_cast<TypeProperty*>(reinterpret_cast<uint64>(InstanceClass) + Offset);
				if (Variable)
				{
					*Variable = NewValur;
				}
			}

		};

	}
}