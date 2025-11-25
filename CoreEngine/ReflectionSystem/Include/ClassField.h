#pragma once
#include <ReflectionSystem/Include/BaseField.h>

#include <ReflectionSystem/Include/ConstructionField.h>




enum class EClassFieldParams : uint8
{
	NONE = 0
};
namespace CoreEngine
{
	struct InitializeObject;

	namespace Reflection
	{
		struct PropertyField;

		struct ClassField : public ConstructionField
		{
			EClassFieldParams Params;
			ClassField* ParentClass = nullptr;

		public:

			virtual PropertyField* GetPropertyFieldByName(void* InstanceClass, const String& NameProperty) const override
			{
				for (PropertyField* Property : PropertyFileds)
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

			bool IsChildClassOf(const ClassField* OtherClass)
			{
				if (!OtherClass) return false;

				ClassField* CurrentClass = this;
				while (CurrentClass)
				{
					if (*CurrentClass == *OtherClass)
					{
						return true;
					}
					CurrentClass = CurrentClass->ParentClass;
				}
				return false;
			}

			virtual void ConstructInstanceObject(Runtime::Object*, const CoreEngine::InitializeObject&) = 0;

		public:

			bool operator==(const ClassField& Other) const
			{
				bool ResEq = true;
				/*if (ParentClass && Other.ParentClass)
				{
					ResEq = *ParentClass == *Other.ParentClass;					
				}
				else if (ParentClass)*/

				ResEq = ConstructionField::operator==(Other);
				if (!ResEq) return false;

				ResEq = Params == Other.Params;
				
				return ResEq;
			}

		};
	}
}