#include <Render/includes/Material.h>
#include <Render/includes/Shader.h>

RMaterial::RMaterial(const CoreEngine::InitializeObject& Initilize) : MaterialInterface(Initilize)
{
}

// void Material::SetShader(CoreEngine::Render::Shader* NewShader)
//{
//	const auto& Uniforms = NewShader->GetAllUniforms();
//	for (auto& Uniform : Uniforms)
//	{
//		UniquePtr<CoreEngine::Render::BaseMaterialProperty> Property = CreatePropertyFromType(Uniform.second.Type);
//		if (Property)
//		{
//			Property->Name = Uniform.first;
//			m_MaterialProperties.emplace_back(std::move(Property));
//		}
//	}
// }

const CoreEngine::Render::RHI::ShaderHandle& RMaterial::GetShaderHandle() const
{
	return m_HandleShader;
}

const ShaderAsset* RMaterial::GetShaderAsset() const
{
	return m_ShaderAsset;
}

EShaderRenderType RMaterial::GetModeRender() const
{
	return m_ModeRender;
}

void RMaterial::SetShaderAsset(ShaderAsset* NewShaderAsset)
{
	m_ShaderAsset = NewShaderAsset;
}

void RMaterial::SetModeRender(const EShaderRenderType NewMode)
{
	m_ModeRender = NewMode;
}

void RMaterial::SetShaderHandle(const CoreEngine::Render::RHI::ShaderHandle& NewHandle)
{
	m_HandleShader = NewHandle;
}


