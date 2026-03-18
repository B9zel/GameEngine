#pragma once
#include <ReflectionSystem/Include/BaseField.h>

#include <ReflectionSystem/Include/ConstructionField.h>




enum EClassFieldParams : uint64
{
	NONE = 0,
	EditorComponent = FLAG_OFFSET(0)
};

namespace CoreEngine
{
	struct InitializeObject;

	namespace Reflection
	{
		struct PropertyField;

		struct ClassField : public ConstructionField
		{
			ClassField* ParentClass = nullptr;

		public:

			virtual PropertyField* GetPropertyFieldByName(void* InstanceClass, const String& NameProperty) const override;

			bool IsChildClassOf(const ClassField* OtherClass);

			virtual void ConstructInstanceObject(Runtime::Object*, const CoreEngine::InitializeObject&) = 0;

			virtual void ValidateMetaClass() override;

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

				ResEq = ParamFlags == Other.ParamFlags;
				
				return ResEq;
			}

		};
	}
}