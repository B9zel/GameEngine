#include <ReflectionSystem/Include/ReflectionManager.h>
#include <ReflectionSystem/Include/ClassField.h>




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
			for (auto& Field : NewClass->PropertyFileds)
			{
				//Field
			}


			FieldsClass.emplace(NameClass, NewClass);
		}
		ClassField* ReflectionManager::FindMetaClass(const String& NameClass)
		{
			auto& FindedClass = FieldsClass.find(NameClass);
			if (FindedClass != FieldsClass.end())
			{
				return FindedClass->second.get();
			}
			return nullptr;
		}

		const DArray<WeakPtr<ClassField>>& ReflectionManager::GetAllClasses() const
		{
			static DArray<WeakPtr<ClassField>> Classes;
			Classes.clear();
			Classes.reserve(FieldsClass.size());
			
			for (auto& Pair : FieldsClass)
			{
				Classes.emplace_back(Pair.second);
			}
			return Classes;
		}
		bool ReflectionManager::HasPropertyField(const String& NameProperty) const
		{
			auto& ClassesArr = GetAllClasses();
			for (auto& Class : ClassesArr)
			{
				auto& ClassObj = Class.lock();
				for (auto& Property : ClassObj->PropertyFileds)
				{
					if (Property->Name == NameProperty)
					{
						return true;
					}
				}
			}
			return false;
		}
	}
}