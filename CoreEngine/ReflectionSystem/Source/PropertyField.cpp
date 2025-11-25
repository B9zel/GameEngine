#include <ReflectionSystem/Include/PropertyField.h>
#include <ReflectionSystem/Include/ConstructionField.h>
#include <Core/includes/Engine.h>
#include <ReflectionSystem/Include/ReflectionManager.h>
#include <Core/includes/StringUtil.h>
#include <ReflectionSystem/Include/ClassField.h>
#include <Runtime/CoreObject/Include/Object.h>
#include <Core/includes/Memory/SerializeArchive.h>
#include <Math/includes/Transform.h>


namespace CoreEngine
{
	namespace Reflection
	{
		static HashTableMap<StringView, EPrimitiveTypes> StoreEnumName = {
				{"bool", EPrimitiveTypes::BOOL},
				{"char", EPrimitiveTypes::CHAR},
				{"float", EPrimitiveTypes::FLOAT_SINGLE},
				{"double", EPrimitiveTypes::FLOAT_DOUBLE},
				{"int8", EPrimitiveTypes::INT8},
				{"int16", EPrimitiveTypes::INT16},{"short", EPrimitiveTypes::INT16},
				{"int32",EPrimitiveTypes::INT32}, {"int",EPrimitiveTypes::INT32},
				{"int64", EPrimitiveTypes::INT64},{"long long", EPrimitiveTypes::INT64},
				{"uint8", EPrimitiveTypes::UINT8},{"unsigned char", EPrimitiveTypes::UINT8},
				{"uint16", EPrimitiveTypes::UINT16},{"unsigned short", EPrimitiveTypes::UINT16},
				{"uint32", EPrimitiveTypes::UINT32},{"unsigned int", EPrimitiveTypes::UINT32},
				{"uint64", EPrimitiveTypes::UINT64},{"unsigned long long", EPrimitiveTypes::UINT64},
				{"String", EPrimitiveTypes::STRING},
				{"FVector", EPrimitiveTypes::VECTOR3F },
				{"FTransform", EPrimitiveTypes::TRANSFORM }
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
				SourceType = Utils::ExtractStrBetweenStr(Type, "<", ">");
				SourceType = Utils::ExtractRightSubStrFindLast(SourceType, "::");
				SourceType = Utils::ExtractLeftSubStrFindLast(SourceType, "*");
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

		ETypeOfPropertyType SimplePropertyTypeField::GetTypeOfPropertyType() const
		{
			return ETypeOfPropertyType::SIMPLE;
		}
		EPrimitiveTypes SimplePropertyTypeField::GetPropertyType() const
		{
			return Primitive;
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
		// Begin ComplexPropertyTypeField
		String ComplexPropertyTypeField::GetNameType() const
		{
			return TypeField ? TypeField->Name : "";
		}
		bool ComplexPropertyTypeField::GetIsPointer() const
		{
			return true;
		}
		ETypeOfPropertyType ComplexPropertyTypeField::GetTypeOfPropertyType() const
		{
			return ETypeOfPropertyType::COMPLEX;
		}
		// End ComplexPropertyTypeField

		// Begin PropertyField
		bool PropertyField::GetIsPointer() const
		{
			return TypeProperty->GetIsPointer();
		}

		bool PropertyField::GetIsSupportReflectionSystem() const
		{
			return CoreEngine::Engine::Get()->GetReflectionManger()->HasPropertyField(Name);
		}

		EConteinType PropertyField::GetPrimitiveType() const
		{
			return EConteinType::PRIMITIVE;
		}

		BaseTypePropertyType* PropertyField::GetTypeProperty()
		{
			return TypeProperty.get();
		}

		void PropertyField::Serialize(SerializeAchive& Archive, Runtime::Object* Instance)
		{
			if (GetIsPointer())
			{
				auto* Obj = GetSourcePropertyByName<Runtime::Object*>(Instance);
				if (Obj && *Obj)
				{
					(*Obj)->Serialize(Archive);
				}
			}
			else
			{
				SerializeDefinitionType(Archive, Instance);
			}
		}
		void PropertyField::SerializeDefinitionType(SerializeAchive& Archive, Runtime::Object* Instance)
		{
			auto* SimpleType = dynamic_cast<SimplePropertyTypeField*>(TypeProperty.get());
			if (!SimpleType) return;

			switch (SimpleType->GetPropertyType())
			{
			case CoreEngine::Reflection::EPrimitiveTypes::NONE:
				break;
			case CoreEngine::Reflection::EPrimitiveTypes::INT8:
			{
				auto* Property = GetSourcePropertyByName<int8>(Instance);
				Archive.SerializeData(Name, *Property);
				break;
			}
			case CoreEngine::Reflection::EPrimitiveTypes::INT16:
			{
				auto* Property = GetSourcePropertyByName<int16>(Instance);
				Archive.SerializeData(Name, *Property);
				break;
			}
			case CoreEngine::Reflection::EPrimitiveTypes::INT32:
			{
				auto* Property = GetSourcePropertyByName<int32>(Instance);
				Archive.SerializeData(Name, *Property);
				break;
			}
			case CoreEngine::Reflection::EPrimitiveTypes::INT64:
			{
				auto* Property = GetSourcePropertyByName<int64>(Instance);
				Archive.SerializeData(Name, *Property);
				break;
			}
			case CoreEngine::Reflection::EPrimitiveTypes::UINT8:
			{
				auto* Property = GetSourcePropertyByName<uint8>(Instance);
				Archive.SerializeData(Name, *Property);
				break;
			}
			case CoreEngine::Reflection::EPrimitiveTypes::UINT16:
			{
				auto* Property = GetSourcePropertyByName<int16>(Instance);
				Archive.SerializeData(Name, *Property);
				break;
			}
			case CoreEngine::Reflection::EPrimitiveTypes::UINT32:
			{
				auto* Property = GetSourcePropertyByName<int32>(Instance);
				Archive.SerializeData(Name, *Property);
				break;
			}
			case CoreEngine::Reflection::EPrimitiveTypes::UINT64:
			{
				auto* Property = GetSourcePropertyByName<uint64>(Instance);
				Archive.SerializeData(Name, *Property);
				break;
			}
			case CoreEngine::Reflection::EPrimitiveTypes::FLOAT_SINGLE:
			{
				auto* Property = GetSourcePropertyByName<float>(Instance);
				Archive.SerializeData(Name, ToString(*Property));
				break;
			}
			case CoreEngine::Reflection::EPrimitiveTypes::FLOAT_DOUBLE:
			{
				auto* Property = GetSourcePropertyByName<double>(Instance);
				Archive.SerializeData(Name, ToString(*Property));
				break;
			}
			case CoreEngine::Reflection::EPrimitiveTypes::STRING:
			{
				auto* Property = GetSourcePropertyByName<String>(Instance);
				Archive.SerializeData(Name, *Property);
				break;
			}
			case CoreEngine::Reflection::EPrimitiveTypes::BOOL:
			{
				auto* Property = GetSourcePropertyByName<bool>(Instance);
				Archive.SerializeData(Name, *Property);
				break;
			}
			case CoreEngine::Reflection::EPrimitiveTypes::CHAR:
			{
				auto* Property = GetSourcePropertyByName<int8>(Instance);
				Archive.SerializeData(Name, *Property);
				break;
			}
			case CoreEngine::Reflection::EPrimitiveTypes::VECTOR3F:
			{
				auto* Property = GetSourcePropertyByName<FVector>(Instance);
				Archive.SerializeData(Name, *Property);
				break;
			}
			case CoreEngine::Reflection::EPrimitiveTypes::TRANSFORM:
			{
				auto* Property = GetSourcePropertyByName<FTransform>(Instance);
				Archive.SerializeData(Name, *Property);
				break;
			}
			default:
				break;
			}
		}
		// End PropertyField

		// Begin ArrayPropertyField
		EConteinType ArrayPropertyField::GetPrimitiveType() const
		{
			return EConteinType::ARRAY;
		}

		void ArrayPropertyField::Serialize(SerializeAchive& Archive, Runtime::Object* Instance)
		{
			if (GetTypeProperty()->GetTypeOfPropertyType() == ETypeOfPropertyType::COMPLEX)
			{
				int64 Size = GetSizeArray<Runtime::Object*>(Instance);
				for (uint64 i = 0; i < Size; i++)
				{
					auto* Obj = GetElement<Runtime::Object*>(Instance, i);
					if (Obj)
					{
						(*Obj)->Serialize(Archive);
					}
				}
			}
			else
			{
				auto* SimpleType = dynamic_cast<SimplePropertyTypeField*>(TypeProperty.get());
				
				SerializedDefenition(SimpleType->GetPropertyType(), Archive, Instance);
			}
		}

		void ArrayPropertyField::SerializedDefenition(EPrimitiveTypes Type, SerializeAchive& Archive, Runtime::Object* Instance)
		{
			int64 Size = GetSizeArray<Runtime::Object*>(Instance);
			for (uint64 i = 0; i < Size; i++)
			{
				switch (Type)
				{
				case CoreEngine::Reflection::EPrimitiveTypes::NONE:
					break;
				case CoreEngine::Reflection::EPrimitiveTypes::INT8:
				{
					auto* Property = GetElement<int8>(Instance, i);
					Archive.SerializeData(Name, *Property);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::INT16:
				{
					auto* Property = GetElement<int16>(Instance, i);
					Archive.SerializeData(Name, *Property);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::INT32:
				{
					auto* Property = GetElement<int32>(Instance, i);
					Archive.SerializeData(Name, *Property);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::INT64:
				{
					auto* Property = GetElement<int64>(Instance, i);
					Archive.SerializeData(Name, *Property);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::UINT8:
				{
					auto* Property = GetElement<uint8>(Instance, i);
					Archive.SerializeData(Name, *Property);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::UINT16:
				{
					auto* Property = GetElement<int16>(Instance, i);
					Archive.SerializeData(Name, *Property);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::UINT32:
				{
					auto* Property = GetElement<int32>(Instance, i);
					Archive.SerializeData(Name, *Property);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::UINT64:
				{
					auto* Property = GetElement<uint64>(Instance, i);
					Archive.SerializeData(Name, *Property);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::FLOAT_SINGLE:
				{
					auto* Property = GetElement<float>(Instance, i);
					Archive.SerializeData(Name, *Property);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::FLOAT_DOUBLE:
				{
					auto* Property = GetElement<double>(Instance, i);
					Archive.SerializeData(Name, *Property);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::STRING:
				{
					auto* Property = GetElement<String>(Instance, i);
					Archive.SerializeData(Name, *Property);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::BOOL:
				{
					bool Property = GetElement(Instance, i);
			
					Archive.SerializeData(Name, Property);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::CHAR:
				{
					auto* Property = GetElement<int8>(Instance, i);
					Archive.SerializeData(Name, *Property);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::VECTOR3F:
				{
					auto* Property = GetElement<FVector>(Instance, i);
					Archive.SerializeData(Name, *Property);
					break;
				}
				case CoreEngine::Reflection::EPrimitiveTypes::TRANSFORM:
				{
					auto* Property = GetElement<FTransform>(Instance, i);
					Archive.SerializeData(Name, *Property);
					break;
				}
				default:
					break;
				}
			}
		}

	}
}