#pragma once
#include <ReflectionSystem/Include/BaseField.h>

namespace Editor
{
	template <class T> 
	void SetPropertyValue(void* Instance, CoreEngine::Reflection::PropertyField* Property, const T& NewValue, const T& OldValue)
	{
		Object* ObjectInstance = static_cast<Object*>(Instance);

		if (NewValue == OldValue || !ObjectInstance || !Property) return;

		ObjectInstance->PreEditChangeProperty(*Property);
		Property->SetSourceProperty<T>(ObjectInstance, NewValue);
		ObjectInstance->PostEditChangeProperty(*Property);
	}
} // namespace Editor
