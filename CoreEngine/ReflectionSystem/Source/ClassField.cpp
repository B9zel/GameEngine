#include <ReflectionSystem/Include/ClassField.h>
#include <ReflectionSystem/Include/PropertyField.h>
#include <Runtime/includes/ActorComponent.h>


namespace CoreEngine::Reflection
{
	PropertyField* CoreEngine::Reflection::ClassField::GetPropertyFieldByName(void* InstanceClass, const String& NameProperty) const
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

	bool ClassField::IsChildClassOf(const ClassField* OtherClass)
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

	void ClassField::ValidateMetaClass()
	{
		if (HasFlag(ParamFlags, static_cast<uint64>(EClassFieldParams::EditorComponent)))
		{
			if (Name == "ActorComponent")
			{
				RemoveFlag(ParamFlags, static_cast<uint64>(EClassFieldParams::EditorComponent));
				return;
			}

			if (!IsChildClassOf(CoreEngine::Runtime::ActorComponent::GetStaticClass()))
			{
				RemoveFlag(ParamFlags, static_cast<uint64>(EClassFieldParams::EditorComponent));
			}
		}
	}

}