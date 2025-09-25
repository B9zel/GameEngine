#include <ReflectionSystem/Include/PropertyField.h>
#include <ReflectionSystem/Include/ConstructionField.h>


namespace CoreEngine
{
	namespace Reflection
	{
		EPrimitiveTypes ConvertToPropertyEnumFromString(const StringView& NameType)
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

			auto& FindedElement = StoreEnumName.find(NameType);
			if (FindedElement != StoreEnumName.end())
			{
				return FindedElement->second;
			}
			return EPrimitiveTypes::NONE;
			
		}
	}
}