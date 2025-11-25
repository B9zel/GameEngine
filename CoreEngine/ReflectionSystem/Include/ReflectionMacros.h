#pragma once
#include <Core/includes/Base.h>
#include <Core/includes/Engine.h>
#include <ReflectionSystem/Include/ReflectionManager.h>
#include <ReflectionSystem/Include/ClassField.h>
#include <ReflectionSystem/Include/ReflectionManager.h>


namespace CoreEngine
{
    struct InitializeObject;
}

namespace CoreEngine::Runtime
{
    class Object;
}


#define RPROPERTY(...) 
#define RCLASS(...) 


#define DEF_COMBINE_FOUR_DETAILS(A, B, C, D) COMBINE_FOUR_DETAILS(A, B, C, D)
#define COMBINE_FOUR_DETAILS(A, B, C, D) A##B##C##D
#define CURRENT_FILE_ID 

#define GENERATED_BODY() DEF_COMBINE_FOUR_DETAILS(CURRENT_FILE_ID, _, __LINE__, _GENERATED_BODY)



//public:					\
//    static CoreEngine::Reflection::ClassField* GetStaticClass(); \
//private:


#define DeclareNewClass(NameNewClass) \
        struct NameNewClass : public CoreEngine::Reflection::ClassField			\
        {                                                                       \
        public:                                                                 \
                                                                                 \
            static DArray<UniquePtr<CoreEngine::Reflection::PropertyField>> PASTE_DETAILS(NameNewClass,_Fields);  \
            virtual void ConstructInstanceObject(CoreEngine::Runtime::Object* Instance, const CoreEngine::InitializeObject& Initilizer) override;                   \
                                                                                \
        public:																	\
            NameNewClass();															\
                                                                            \
        };

#define ImplementNewClass(NameNewClass, NameClass, Namespace, ClassFieldParams, SizeOfClass, FieldsOfClass, Parent) \
        NameNewClass::NameNewClass() \
        {																	\
                Name = #NameClass;												\
                Params = ClassFieldParams;									\
                TypeConstruction = CoreEngine::Reflection::ETypeConstructionFiled::CLASS;  \
                Size = SizeOfClass;											            \
                ParentClass = Parent;                                                   \
                for (int64 i = 0; i < FieldsOfClass.size(); ++i)     \
                {                                                                          \
                    PropertyFileds.emplace_back(FieldsOfClass[i].get());                         \
                }                                                                           \
        }	                                                                                            \
        void NameNewClass::ConstructInstanceObject(CoreEngine::Runtime::Object* Instance, const CoreEngine::InitializeObject& Initilizer)  \
        {                                                                                                           \
            new(Instance) Namespace::NameClass(Initilizer);                                                                     \
        }                                                                                                             \

#define ImplementStaticClass(ClassOfMethod, NewClass, NameSourceClass) \
            CoreEngine::Reflection::ClassField* ClassOfMethod::GetStaticClass() \
            {																			 \
                static WeakPtr<CoreEngine::Reflection::ClassField> StaticClassInstance;\
                if (StaticClassInstance.expired()) {												 \
                    auto NewMetaClass = MakeSharedPtr<NewClass>();						\
                    CoreEngine::Engine::Get()->GetReflectionManger()->RegisterNewClass(NameSourceClass, NewMetaClass); \
                    StaticClassInstance = NewMetaClass;								 \
                }																		 \
                return StaticClassInstance.lock().get();											     \
            };

#define GeneratePropertyFiled(VarName, Type, OffsetFromObject, IsPointerField, Param)       \
           struct PASTE_DETAILS(Field_, VarName) : public CoreEngine::Reflection::PropertyField	\
           {                                                                                    \
               PASTE_DETAILS(Field_, VarName)() {                                            \
                     SizeByte = sizeof(Type);                                                      \
                     Name = STRINGCON_DETAILS(VarName);                                        \
                     Offset = OffsetFromObject;                                              \
                                                             \
                     Params = Param;                                                        \
                     TypeProperty = CoreEngine::Reflection::BaseTypePropertyType::Create(STRINGCON_DETAILS(Type));  \
               }                                                           \
               PASTE_DETAILS(Field_, VarName)(const PASTE_DETAILS(Field_, VarName)& Other) = default; \
               PASTE_DETAILS(Field_, VarName)& operator=(const PASTE_DETAILS(Field_, VarName)& Other) = default; \
           };                                                      \

#define GenerateClassPropertyFiled(VarName, Type, OffsetFromObject, Param)       \
           struct PASTE_DETAILS(Field_, VarName) : public CoreEngine::Reflection::PropertyField	\
           {                                                                                    \
               PASTE_DETAILS(Field_, VarName)() {                                            \
                     SizeByte = sizeof(Type*);                                                      \
                     Name = STRINGCON_DETAILS(VarName);                                        \
                     Offset = OffsetFromObject;                                              \
                                                             \
                     Params = Param;                                                        \
                     TypeProperty = CoreEngine::Reflection::BaseTypePropertyType::Create(Type::GetStaticClass());  \
               }                                                           \
               PASTE_DETAILS(Field_, VarName)(const PASTE_DETAILS(Field_, VarName)& Other) = default; \
               PASTE_DETAILS(Field_, VarName)& operator=(const PASTE_DETAILS(Field_, VarName)& Other) = default; \
           };   



#define GenerateArrayPropertyFiled(VarName, Type, OffsetFromObject, IsPointerField, Param)       \
           struct PASTE_DETAILS(Field_, VarName) : public CoreEngine::Reflection::ArrayPropertyField	\
           {                                                                                    \
               PASTE_DETAILS(Field_, VarName)() {                                            \
                     SizeByte = sizeof(Type);                                                      \
                     Name = STRINGCON_DETAILS(VarName);                                        \
                     Offset = OffsetFromObject;                                              \
                                                             \
                     Params = Param;                                                        \
                     TypeProperty = CoreEngine::Reflection::BaseTypePropertyType::Create(STRINGCON_DETAILS(Type));  \
               }                                                           \
               PASTE_DETAILS(Field_, VarName)(const PASTE_DETAILS(Field_, VarName)& Other) = default; \
               PASTE_DETAILS(Field_, VarName)& operator=(const PASTE_DETAILS(Field_, VarName)& Other) = default; \
           };  

#define GenerateClassArrayPropertyFiled(VarName, TemplateType, InnerType, OffsetFromObject, IsPointerField, Param)       \
           struct PASTE_DETAILS(Field_, VarName) : public CoreEngine::Reflection::ArrayPropertyField	                    \
           {                                                                                                                 \
               PASTE_DETAILS(Field_, VarName)() {                                                                            \
                     SizeByte = sizeof(TemplateType);                                                                         \
                     Name = STRINGCON_DETAILS(VarName);                                                                        \
                     Offset = OffsetFromObject;                                                                                 \
                                                                                                                                 \
                     Params = Param;                                                                                             \
                               \
               }                                                           \
               PASTE_DETAILS(Field_, VarName)(const PASTE_DETAILS(Field_, VarName)& Other) = default; \
               PASTE_DETAILS(Field_, VarName)& operator=(const PASTE_DETAILS(Field_, VarName)& Other) = default; \
               virtual CoreEngine::Reflection::BaseTypePropertyType* GetTypeProperty() override \
               {                                                                                \
                   if (!TypeProperty)                                                           \
                   {                                                                             \
                       TypeProperty = CoreEngine::Reflection::BaseTypePropertyType::Create(InnerType::GetStaticClass()); \
                   }                                                                                                    \
                   return TypeProperty.get();                                                                             \
               }                                                                                                            \
           };  

#define GenetateRegistryClass(NameClass, Namespace) \
    namespace {                             \
        struct PASTE_DETAILS(RegistryGenerate_, NameClass) \
        {                                                               \
            PASTE_DETAILS(RegistryGenerate_, NameClass)() {               \
                                                                        \
                CoreEngine::Reflection::RegistryClass Class;                                    \
                Class.CreateMetaClass = []()  -> CoreEngine::Reflection::ConstructionField*                          \
                {                                                       \
                    return Namespace::##NameClass::GetStaticClass();                 \
                };                                                                  \
                Class.ConstructClass = [](CoreEngine::Runtime::Object* ConstructObj, const CoreEngine::InitializeObject& Initialize)    \
                    {                                                                                                       \
                        new(ConstructObj) Namespace::##NameClass(Initialize);                                                    \
                    }; \
                CoreEngine::Reflection::MapRegistryClass::Instance().Register(#NameClass, std::move(Class)); \
            }                                                                           \
        };                                                                              \
        static PASTE_DETAILS(RegistryGenerate_, NameClass) PASTE_DETAILS(_AutoRegistry_, NameClass);   \
    }