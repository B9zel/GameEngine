#pragma once
#include <ReflectionSystem/Include/PropertyField.h>
#include <ReflectionSystem/Include/BaseField.h>

namespace CoreEngine
{
	namespace Reflection
	{
		struct BaseField;
		struct PropertyField;

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


			virtual PropertyField* GetPropertyFieldByName(void* InstanceClass, const String& NameProperty)
			{
				for (PropertyField*& Property : PropertyFileds)
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
		};
	}
}