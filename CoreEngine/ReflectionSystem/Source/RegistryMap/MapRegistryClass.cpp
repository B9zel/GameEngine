#include <ReflectionSystem/Include/RegistryMap/MapRegistryClass.h>
#include <ReflectionSystem/Include/ClassField.h>
#include <Runtime/CoreObject/Include/Object.h>

namespace CoreEngine
{
	namespace Reflection
	{

		MapRegistryClass& MapRegistryClass::Instance()
		{
			static MapRegistryClass Instance;
			return Instance;
		}

		ConstructionField* MapRegistryClass::GetConstructionField(const String& NameField) const
		{
			auto& ConstructField = m_RegistryClasses.find(NameField);
			if (ConstructField != m_RegistryClasses.end())
			{
				return ConstructField->second.CreateMetaClass.Invoke();
			}
			return nullptr;
		}


		ClassField* MapRegistryClass::GetClassField(const String& NameClass) const
		{
			return dynamic_cast<ClassField*>(GetConstructionField(NameClass));
		}

		void MapRegistryClass::Register(const String& Name, RegistryClass&& NewField)
		{
			m_RegistryClasses.emplace(Name, NewField);
		}
		HashTableMap<String, RegistryClass>& MapRegistryClass::GetData()
		{
			return m_RegistryClasses;
		}
	}
}