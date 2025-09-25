#pragma once
#include <Core/includes/Base.h>
#include <Core/includes/Engine.h>
#include <ReflectionSystem/Include/ReflectionManager.h>
#include <ReflectionSystem/Include/ClassField.h>
#include <ReflectionSystem/Include/ReflectionManager.h>


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
                                                                                \
        public:																	\
            NameNewClass();															\
                                                                            \
        };

#define ImplementNewClass(NameNewClass, NameClass, ClassFieldParams, SizeOfClass, FieldsOfClass, Parent) \
        NameNewClass::NameNewClass() \
        {																	\
                Name = NameClass;												\
                Params = ClassFieldParams;									\
                TypeConstruction = CoreEngine::Reflection::ETypeConstructionFiled::CLASS;  \
                Size = SizeOfClass;											            \
                ParentClass = Parent;                                                   \
                for (int64 i = 0; i < FieldsOfClass.size(); ++i)     \
                {                                                                          \
                    PropertyFileds.emplace_back(FieldsOfClass[i].get());                         \
                }                                                                           \
        }	

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
                     Size = sizeof(Type);                                                      \
                     Name = STRINGCON_DETAILS(VarName);                                        \
                      Offset = OffsetFromObject;                                              \
                     IsPointer = IsPointerField;                                            \
                     Params = Param;                                                        \
                     TypeProperty = CoreEngine::Reflection::TypePropertyField(STRINGCON_DETAILS(Type));  \
               }                                                           \
               PASTE_DETAILS(Field_, VarName)(const PASTE_DETAILS(Field_, VarName)& Other) = default; \
               PASTE_DETAILS(Field_, VarName)& operator=(const PASTE_DETAILS(Field_, VarName)& Other) = default; \
           };                                                      \


