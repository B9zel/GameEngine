#pragma once
#include <Core/includes/Base.h>
#include <ReflectionSystem/Include/BaseField.h>

//#include <ReflectionSystem/Include/ClassField.h>
//#include <ReflectionSystem/Include/ConstructionField.h>
#include <ReflectionSystem/Include/ReflectionManager.h>



namespace CoreEngine
{
	class Engine;

	namespace Reflection
	{
		struct ConstructionField;
		class ReflectionManager;
		struct ClassField;

		enum class ETypeOfPropertyType : uint8
		{
			SIMPLE = 0,
			COMPLEX
		};

		enum class EConteinType : uint8
		{
			NONE = 0,
			PRIMITIVE,
			ARRAY
		};

		enum class EPropertyFieldParams : uint32
		{
			NONE = 0,
			EditorVisible = FLAG_OFFSET(0)
		};

		enum class EPrimitiveTypes : uint8
		{
			NONE = 0,

			INT8,
			INT16,
			INT32,
			INT64,
			UINT8,
			UINT16,
			UINT32,
			UINT64,
			FLOAT_SINGLE,
			FLOAT_DOUBLE,
			STRING,
			BOOL,
			CHAR,
			VECTOR3F,
			TRANSFORM
		};

		EPrimitiveTypes ConvertToPropertyEnumFromString(const String& NameType);
		bool GetIsSupportSimpleTypeFromString(const String& NameType);

		struct BaseTypePropertyType
		{
			static SharedPtr<BaseTypePropertyType> Create(const String& Type);
			static SharedPtr<BaseTypePropertyType> Create(ClassField* TypeField);
			
			virtual String GetNameType() const = 0;
			virtual bool GetIsPointer() const = 0;
			virtual ETypeOfPropertyType GetTypeOfPropertyType() const = 0;
		};

		struct SimplePropertyTypeField : public BaseTypePropertyType
		{

			SimplePropertyTypeField() = default;
			~SimplePropertyTypeField() noexcept {}
			SimplePropertyTypeField(const String& Type);
			
		public:

			SimplePropertyTypeField(const SimplePropertyTypeField& Other) = default;
			SimplePropertyTypeField( SimplePropertyTypeField&& Other) = default;
			SimplePropertyTypeField& operator=(SimplePropertyTypeField&&) = default;
			SimplePropertyTypeField& operator=(const SimplePropertyTypeField& Other);

		public:

			virtual String GetNameType() const override;
			virtual bool GetIsPointer() const;
			virtual ETypeOfPropertyType GetTypeOfPropertyType() const override;

		private:

			String& RemoveSpace(String& Str);

		public:

			EPrimitiveTypes Primitive;
		
			String NameType;
		};

		struct ComplexPropertyTypeField : public BaseTypePropertyType
		{
		public:

			ComplexPropertyTypeField(const String& Type);// : BaseTypePropertyType()
			ComplexPropertyTypeField(ClassField* Field);// : BaseTypePropertyType()
			

		public:

			virtual String GetNameType() const override;
			virtual bool GetIsPointer() const;
			virtual ETypeOfPropertyType GetTypeOfPropertyType() const override;

		public:

			ConstructionField* TypeField;
		};


		struct PropertyField : public BaseField
		{

			virtual ~PropertyField() noexcept {
			}
		public:

			PropertyField() = default;
			PropertyField(const PropertyField& Other)
			{
				SizeByte = Other.SizeByte;
				Offset = Other.Offset;
				Params = Other.Params;
				TypeProperty = Other.TypeProperty;
			}

			PropertyField& operator=(const PropertyField& Other)
			{
				SizeByte = Other.SizeByte;
				Offset = Other.Offset;
				Params = Other.Params;
				TypeProperty = Other.TypeProperty;
				
				return *this;
			}

		public:

			template<class TypeProperty>
			TypeProperty* GetSourcePropertyByName(void* InstanceClass)
			{
				return reinterpret_cast<TypeProperty*>(reinterpret_cast<uint64>(InstanceClass) + Offset);
			}

			template<typename TypeProperty>
			void SetSourceProperty(void* InstanceClass, const TypeProperty NewValur)
			{
				TypeProperty* Variable = reinterpret_cast<TypeProperty*>(reinterpret_cast<uint64>(InstanceClass) + Offset);
				if (Variable)
				{
					*Variable = NewValur;
				}
			}

			bool GetIsPointer() const
			{
				return TypeProperty->GetIsPointer();
			}

			bool GetIsSupportReflectionSystem() const;
			virtual EConteinType GetPrimitiveType() const;

		public:

			uint32 SizeByte;
			uint32 Offset;
			EPropertyFieldParams Params;
			SharedPtr<BaseTypePropertyType> TypeProperty;
		};

		struct ArrayPropertyField : public PropertyField
		{
		public:

			ArrayPropertyField() = default;

		public:

			template<class TypeStorage>
			TypeStorage* GetElement(void* Instance, const uint32 Index)
			{
				DArray<TypeStorage>* Variable = reinterpret_cast<DArray<TypeStorage>*>(reinterpret_cast<uint64>(Instance) + Offset);
				if (Variable)
				{
					return &((*Variable)[Index]);
				}
				return nullptr;
			}

			template<class TypeStorage>
			int64 GetSizeArray(void* Instance)
			{
				DArray<TypeStorage>* Variable = reinterpret_cast<DArray<TypeStorage>*>(reinterpret_cast<uint64>(Instance) + Offset);
				if (Variable)
				{
					return Variable->size();
				}
				return -1;
			}

			virtual EConteinType GetPrimitiveType() const override;
		};
	}


}
