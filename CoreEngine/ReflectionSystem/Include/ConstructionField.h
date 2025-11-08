#pragma once
#include <ReflectionSystem/Include/BaseField.h>
#include <ReflectionSystem/Include/PropertyField.h>

namespace CoreEngine
{
	namespace Reflection
	{
		/*struct BaseField;
		struct PropertyField;*/

		enum class ETypeConstructionFiled : uint8
		{
			STRUCT = 0,
			CLASS
		};
		struct ConstructionField : public BaseField
		{
			uint32 Size;
			DArray<PropertyField*> PropertyFileds;
			ETypeConstructionFiled TypeConstruction;

		public:

			virtual PropertyField* GetPropertyFieldByName(void* InstanceClass, const String& NameProperty) const
			{
				for (PropertyField* Property : PropertyFileds)
				{
					if (Property->Name == NameProperty)
					{
						return Property;
					}
				}
				return nullptr;
			}

			void AddPropertyField(PropertyField* NewProperty)
			{
				PropertyFileds.emplace_back(NewProperty);
			}

		public:

			bool operator==(const ConstructionField& Other) const
			{
				bool ResEq = BaseField::operator==(Other);
				if (!ResEq) return false;

				ResEq = Size == Other.Size;
				if (!ResEq) return false;

				ResEq = TypeConstruction == Other.TypeConstruction;
				if (!ResEq) return false;

				return PropertyFileds == Other.PropertyFileds;
			}

		};
	}
}