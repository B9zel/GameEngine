#pragma once
#include <Core/includes/Base.h>

namespace CoreEngine
{
	namespace Reflection
	{
		struct ClassField;
		struct PropertyField;

		class ReflectionManager
		{
		public:

			static UniquePtr<ReflectionManager> CreateReflectionManager();
			void RegisterNewClass(const String& NameClass,const SharedPtr<ClassField>& NewClass);
			ClassField* FindMetaClass(const String& NameClass);
			const DArray<WeakPtr<ClassField>>& GetAllClasses() const;
			bool HasPropertyField(const String& NameProperty) const;

		private:

			ReflectionManager() = default;

		private:

			HashTableMap<String, SharedPtr<ClassField>> FieldsClass;
			static ReflectionManager* Instance;
		};
	}
}