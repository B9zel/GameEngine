#pragma once
#include <Runtime/CoreObject/Include/Object.h>
#include <Render/includes/RenderHandle.h>
#include <Render/includes/Enums/ShaderRenderType.h>
#include <Render/includes/UniformType.h>
#include <Math/includes/Vector.h>
#include <Math/includes/Matrix.h>
#include <MaterialInterface.generated.h>

namespace CoreEngine::Render
{
	struct MaterialHandle : public RenderHandle
	{
	};

	struct BaseMaterialProperty
	{
	public:

		template <class ReturnType> ReturnType* GetValue() const
		{
			constexpr IsValidType = std::is_same_v<ReturnType, float> || std::is_same_v<ReturnType, int32> || std::is_same_v<ReturnType, uint32> ||
									std::is_same_v<ReturnType, FVector> || std::is_same_v<ReturnType, FMatrix4x4>;
			CORE_ASSERT(IsValidType, "Invalid type for material property");

			return static_cast<ReturnType*>(GetProperty());
		}

	protected:

		BaseMaterialProperty(const EUniformType type) : Type{type}
		{
		}
		virtual void* GetProperty() = 0;

	public:

		String Name;
		const EUniformType Type;
	};

	struct MaterialPropertyFloat : public BaseMaterialProperty
	{
	public:

		MaterialPropertyFloat() : BaseMaterialProperty(EUniformType::FLOAT)
		{
		}

	protected:

		virtual void* GetProperty() override
		{
			return &Value;
		}

	public:

		float Value;
	};

	struct MaterialPropertyVec3 : public BaseMaterialProperty
	{
	public:

		MaterialPropertyVec3() : BaseMaterialProperty(EUniformType::VEC3)
		{
		}

	protected:

		virtual void* GetProperty() override
		{
			return &Value;
		}

	public:

		FVector Value = FVector::ZeroVector;
	};

	struct MaterialPropertyMat4 : public BaseMaterialProperty
	{
	public:

		MaterialPropertyMat4() : BaseMaterialProperty(EUniformType::MAT4)
		{
		}

	protected:

		virtual void* GetProperty() override
		{
			return &Value;
		}

	public:

		FMatrix4x4 Value;
	};

	struct MaterialPropertyInt : public BaseMaterialProperty
	{
	public:

		MaterialPropertyInt() : BaseMaterialProperty(EUniformType::INT)
		{
		}

	protected:

		virtual void* GetProperty() override
		{
			return &Value;
		}

	public:

		int32 Value;
	};

	struct MaterialPropertyUInt : public BaseMaterialProperty
	{
	public:

		MaterialPropertyUInt() : BaseMaterialProperty(EUniformType::UINT)
		{
		}

	protected:

		virtual void* GetProperty() override
		{
			return &Value;
		}

	public:

		uint32 Value;
	};

	UniquePtr<CoreEngine::Render::BaseMaterialProperty> CreatePropertyFromType(const EUniformType& Type);

	// namespace Render
} // namespace CoreEngine::Render

RCLASS()
class MaterialInterface : public Object
{
	GENERATED_BODY()

public:

	MaterialInterface(const CoreEngine::InitializeObject& Initilize);
};
