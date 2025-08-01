#pragma once
#include <ReflectionSystem/Include/ClassField.h>


namespace CoreEngine
{
	namespace Reflection
	{
		class MetaClass
		{
		public:

			void SetClassField(ClassField* FieldClass)
			{
				if (!FieldClass) return;

				ClassMeta = FieldClass;
			}

		private:

			ClassField* ClassMeta;
		};
	}
}