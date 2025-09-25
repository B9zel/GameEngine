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
			ClassField* FindMetaClas(const String& NameClass);

		private:

			ReflectionManager() = default;

		private:

			HashTableMap<String, SharedPtr<ClassField>> FieldsClass;
			static ReflectionManager* Instance;
		};
	}
}