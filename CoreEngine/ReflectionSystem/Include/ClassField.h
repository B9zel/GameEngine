#pragma once
#include <ReflectionSystem/Include/BaseField.h>
#include <ReflectionSystem/Include/ConstructionField.h>





enum class EClassFieldParams : uint8
{
	NONE = 0
};
namespace CoreEngine
{
	namespace Reflection
	{

		struct ClassField : public ConstructionField
		{
			EClassFieldParams Params;
			ClassField* ParentClass = nullptr;

			virtual PropertyField* GetPropertyFieldByName(void* InstanceClass, const String& NameProperty) override
			{
				for (PropertyField*& Property : PropertyFileds)
				{
					if (Property->Name == NameProperty)
					{
						return Property;
					}
				}
				if (ParentClass)
				{
					return ParentClass->GetPropertyFieldByName(InstanceClass, NameProperty);
				}
				return nullptr;
			}
		};
	}
}