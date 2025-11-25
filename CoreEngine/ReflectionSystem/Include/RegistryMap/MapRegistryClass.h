#pragma once
#include <Core/includes/Base.h>


namespace CoreEngine
{
	struct InitializeObject;
	namespace Runtime
	{
		class Object;
	}

	namespace Reflection
	{
		struct ConstructionField;
		struct ClassField;

		struct RegistryClass
		{
			RegistryClass() = default;

			RegistryClass(RegistryClass&&) = default;
			RegistryClass(const RegistryClass&) = default;

			RegistryClass& operator=(const RegistryClass&) = default;
			RegistryClass& operator=(RegistryClass&&) = default;

		public:

			FunctionPtr<ConstructionField*()>CreateMetaClass{nullptr};
			FunctionPtr<void(Runtime::Object*, const InitializeObject&)>ConstructClass{nullptr};
		};

		class MapRegistryClass
		{
		public:

			static MapRegistryClass& Instance();
			ConstructionField* GetConstructionField(const String& NameField) const;
			ClassField* GetClassField(const String& NameClass) const;
			void Register(const String& Name, RegistryClass&& NewField);

			HashTableMap<String, RegistryClass>& GetData();

		private:

			HashTableMap<String, RegistryClass> m_RegistryClasses;
		};
	}
}