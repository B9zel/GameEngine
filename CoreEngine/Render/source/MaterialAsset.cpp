#include <Render/includes/MaterialAsset.h>
#include <Render/includes/Shader.h>
#include <Platform/Renderer/OpenGL/include/OpenGLShader.h>
#include <Render/includes/ShaderGLSLUtils.h>
#include <Render/includes/MaterialInterface.h>

MaterialAsset::MaterialAsset(const CoreEngine::InitializeObject& Initilize) : Asset(Initilize)
{
	PostChangeProperty.AddBind(&MaterialAsset::PostChangePropertyPathToAsset, this);
}

CoreEngine::EAssetType MaterialAsset::GetAssetType() const
{
	return CoreEngine::EAssetType::Material;
}

EShaderRenderType MaterialAsset::GetModeRender() const
{
	return EShaderRenderType::LIT;
}

ShaderAsset* MaterialAsset::GetShaderAsset() const
{
	return m_ShaderAsset;
}

void MaterialAsset::SetShaderAsset(ShaderAsset* NewShaderAsset)
{
	if (!NewShaderAsset || NewShaderAsset == m_ShaderAsset) return;

	SetNewShader(NewShaderAsset);
}

const String MaterialAsset::GetPathToShaderAsset() const
{
	return m_ShaderAsset ? m_ShaderAsset->GetPathToAsset() : "";
}

DArray<SharedPtr<CoreEngine::Render::BaseMaterialProperty>>& MaterialAsset::GetShaderUniforms()
{
	return m_ShaderUniforms;
}

void MaterialAsset::PostChangePropertyPathToAsset(Asset* Asset, CoreEngine::Reflection::PropertyField& Property)
{
	if (Property.Name == STRINGCON_DETAILS(m_ShaderAsset))
	{
		if (ShaderAsset* NewShader = dynamic_cast<ShaderAsset*>(m_ShaderAsset))
		{
			NewShader->ApplyNewShader.AddBind(&MaterialAsset::UpdateShaderUniforms, this);
		}

		SetNewShader(m_ShaderAsset);
	}
}

void MaterialAsset::PreChangePropertyPathToAsset(Asset* Asset, CoreEngine::Reflection::PropertyField& Property)
{
	if (Property.Name == STRINGCON_DETAILS(m_ShaderAsset))
	{
		if (ShaderAsset* NewShader = dynamic_cast<ShaderAsset*>(m_ShaderAsset))
		{
			NewShader->ApplyNewShader.Remove(&MaterialAsset::UpdateShaderUniforms, this);
		}

		SetNewShader(m_ShaderAsset);
	}
}

void MaterialAsset::SetNewShader(ShaderAsset* Shader)
{
	if (!Shader) return;

	m_ShaderAsset = Shader;
	PathToShaderAsset = m_ShaderAsset->GetPathToAsset();

	UpdateShaderUniforms();
}

void MaterialAsset::UpdateShaderUniforms()
{
	const SourceShader& Shaders = m_ShaderAsset->GetCustomShader();
	HashTableMap<String, CoreEngine::Render::GLSL::UniformFileInfo> Uniforms;

	CoreEngine::Render::GLSL::ParseShader(Shaders.VertexShader + Shaders.FragmentShader, Uniforms);
	m_ShaderUniforms.clear();

	for (auto& uniform : Uniforms)
	{
		auto Property = CoreEngine::Render::CreatePropertyFromType(uniform.second.Type);
		Property->Name = uniform.first;

		m_ShaderUniforms.push_back(std::move(Property));
	}

	OnUpdateShaderUniform.Call();
}
