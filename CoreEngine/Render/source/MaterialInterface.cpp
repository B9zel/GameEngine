#include <Render/includes/MaterialInterface.h>

MaterialInterface::MaterialInterface(const CoreEngine::InitializeObject& Initilize) : Object(Initilize)
{
}

namespace CoreEngine::Render
{
	UniquePtr<CoreEngine::Render::BaseMaterialProperty> CreatePropertyFromType(const EUniformType& Type)
	{
		switch (Type)
		{
		case EUniformType::INT:
			return MakeUniquePtr<CoreEngine::Render::MaterialPropertyInt>();
		case EUniformType::UINT:
			return MakeUniquePtr<CoreEngine::Render::MaterialPropertyUInt>();
		case EUniformType::FLOAT:
			return MakeUniquePtr<CoreEngine::Render::MaterialPropertyFloat>();
		case EUniformType::VEC3:
			return MakeUniquePtr<CoreEngine::Render::MaterialPropertyVec3>();
		case EUniformType::MAT4:
			return MakeUniquePtr<CoreEngine::Render::MaterialPropertyMat4>();
		default:
			ASSERT("Don't support this type of uniform");
			return nullptr;
		}
	}
} // namespace CoreEngine::Render
