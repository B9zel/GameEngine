#include <ReflectionSystem/Include/ReflectionManager.h>
#include <ReflectionSystem/Include/ClassField.h>
#include <ReflectionSystem/Include/PropertyField.h>



DECLARE_LOG_CATEGORY_EXTERN(ReflectionManagerLog)

namespace CoreEngine
{
	namespace Reflection
	{
		ReflectionManager* ReflectionManager::Instance = nullptr;
		UniquePtr<ReflectionManager> ReflectionManager::CreateReflectionManager()
		{
			class WrapperReflectionManger : public ReflectionManager {};

			if (!Instance)
			{
				auto NewInstance = MakeUniquePtr<WrapperReflectionManger>();
				Instance = NewInstance.get();
				return NewInstance;
			}

			return UniquePtr<ReflectionManager>(Instance);
		}

		void ReflectionManager::RegisterNewClass(const String& NameClass, const SharedPtr<ClassField>& NewClass)
		{
			if (FieldsClass.count(NameClass))
			{
				EG_LOG(ReflectionManagerLog, ELevelLog::ERROR, "{0} already exist", NameClass);
				return;
			}
			FieldsClass.emplace(NameClass, NewClass);
		}
		ClassField* ReflectionManager::FindMetaClas(const String& NameClass)
		{
			auto& FindedClass = FieldsClass.find(NameClass);
			if (FindedClass != FieldsClass.end())
			{
				return FindedClass->second.get();
			}
			return nullptr;
		}
	}
}