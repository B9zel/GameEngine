#include <ReflectionSystem/Include/PropertyField.h>
#include <ReflectionSystem/Include/ConstructionField.h>
#include <Core/includes/Engine.h>
#include <ReflectionSystem/Include/ReflectionManager.h>
#include <Core/includes/StringUtil.h>
#include <ReflectionSystem/Include/ClassField.h>


namespace CoreEngine
{
	namespace Reflection
	{
		static HashTableMap<StringView, EPrimitiveTypes> StoreEnumName = {
				{"bool", EPrimitiveTypes::BOOL},
				{"char", EPrimitiveTypes::CHAR},
				{"float", EPrimitiveTypes::FLOAT_DOUBLE},
				{"double", EPrimitiveTypes::FLOAT_SINGLE},
				{"int8", EPrimitiveTypes::INT8},
				{"int16", EPrimitiveTypes::INT16},{"short", EPrimitiveTypes::INT16},
				{"int32",EPrimitiveTypes::INT32}, {"int",EPrimitiveTypes::INT32},
				{"int64", EPrimitiveTypes::INT64},{"long long", EPrimitiveTypes::INT64},
				{"uint8", EPrimitiveTypes::UINT8},{"unsigned char", EPrimitiveTypes::UINT8},
				{"uint16", EPrimitiveTypes::UINT16},{"unsigned short", EPrimitiveTypes::UINT16},
				{"uint32", EPrimitiveTypes::UINT32},{"unsigned int", EPrimitiveTypes::UINT32},
				{"uint64", EPrimitiveTypes::UINT64},{"unsigned long long", EPrimitiveTypes::UINT64},
				{"String", EPrimitiveTypes::STRING},
		};

		EPrimitiveTypes ConvertToPropertyEnumFromString(const String& NameType)
		{
			auto& FindedElement = StoreEnumName.find(NameType);
			if (FindedElement != StoreEnumName.end())
			{
				return FindedElement->second;
			}
			return EPrimitiveTypes::NONE;
			
		}

		bool GetIsSupportSimpleTypeFromString(const String& NameType)
		{
			return StoreEnumName.count(NameType) >= 1;
		}


		// Begin SimplePropertyField
	
		SimplePropertyTypeField::SimplePropertyTypeField(const String& Type)
		{
			
			// Is pointer
			NameType = Type;
			RemoveSpace(NameType);

			uint64 Pos = NameType.find("*");
			CORE_ASSERT(Pos >= 0, "SimplePropertyField don't support pointer type");

			//IsComplexType = Pos >= 0;
			Primitive = ConvertToPropertyEnumFromString(NameType);
			/*if (Primitive == EPrimitiveTypes::NONE)
			{
				IsComplexType = true;
			}
			else
			{
				IsComplexType = false;
			}
			*/
		}
		SimplePropertyTypeField& SimplePropertyTypeField::operator=(const SimplePropertyTypeField& Other)
		{
			//IsComplexType = Other.IsComplexType;
			Primitive = Other.Primitive;
			NameType = Other.NameType;

			return *this;
			
		}
		SharedPtr<BaseTypePropertyType> BaseTypePropertyType::Create(const String& Type)
		{
			String SourceType;
			StringView FindArray = "DArray";
			int64 PosFindedArray = Type.find(FindArray);
			if (PosFindedArray >= 0)
			{
				SourceType = ExtractStrBetweenStr(Type, "<", ">");
				SourceType = ExtractRightSubStrFindLast(SourceType, "::");
				SourceType = ExtractLeftSubStrFindLast(SourceType, "*");
			}
			else
			{
				SourceType = Type;
			}

			if (GetIsSupportSimpleTypeFromString(SourceType))
			{
				return MakeSharedPtr<SimplePropertyTypeField>(SourceType);
			}
			return MakeSharedPtr<ComplexPropertyTypeField>(SourceType);
		}

		SharedPtr<BaseTypePropertyType> BaseTypePropertyType::Create(ClassField* TypeField)
		{
			return MakeSharedPtr<ComplexPropertyTypeField>(TypeField);

		}

		String SimplePropertyTypeField::GetNameType() const
		{
			return NameType;
		}

		String& SimplePropertyTypeField::RemoveSpace(String& Str)
		{
			int32 Pos = Str.find(' ');
			for (; Pos >= 0; Pos = Str.find(' '))
			{
				Str.erase(Pos, 1);
			}
			return Str;
		}

		bool SimplePropertyTypeField::GetIsPointer() const
		{
			return false;
		}
		// End SimplePropertyField

		ComplexPropertyTypeField::ComplexPropertyTypeField(const String& Type)
		{
			TypeField = Engine::Get()->GetReflectionManger()->FindMetaClass(Type);
			
		}

		ComplexPropertyTypeField::ComplexPropertyTypeField(ClassField* Field)
		{
			TypeField = Field;
		}

		String ComplexPropertyTypeField::GetNameType() const
		{
			return TypeField ? TypeField->Name : "";
		}
		bool ComplexPropertyTypeField::GetIsPointer() const
		{
			return true;
		}

		bool PropertyField::GetIsSupportReflectionSystem() const
		{
			return CoreEngine::Engine::Get()->GetReflectionManger()->HasPropertyField(Name);
		}

		EConteinType PropertyField::GetPrimitiveType() const
		{
			return EConteinType::PRIMITIVE;
		}
		

		// Begin ArrayPropertyField
		EConteinType ArrayPropertyField::GetPrimitiveType() const
		{
			return EConteinType::ARRAY;
		}

	}
}