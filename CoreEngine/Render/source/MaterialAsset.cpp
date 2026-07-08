#include <Render/includes/MaterialAsset.h>
#include <Render/includes/Shader.h>
#include <Platform/Renderer/OpenGL/include/OpenGLShader.h>
#include <Render/includes/ShaderGLSLUtils.h>
#include <Render/includes/MaterialInterface.h>

MaterialAsset::MaterialAsset(const CoreEngine::InitializeObject& Initilize) : Asset(Initilize)
{
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

	m_ShaderAsset = NewShaderAsset;
	PathToShaderAsset = m_ShaderAsset->GetPathToAsset();

	const SourceShader& Shaders = NewShaderAsset->GetCustomShader();
	HashTableMap<String, CoreEngine::Render::GLSL::UniformFileInfo> Uniforms;

	CoreEngine::Render::GLSL::ParseShader(Shaders.VertexShader + Shaders.FragmentShader, Uniforms);

	for (auto& uniform : Uniforms)
	{
		auto Property = CoreEngine::Render::CreatePropertyFromType(uniform.second.Type);
		Property->Name = uniform.first;

		m_ShaderUniforms.push_back(std::move(Property));
	}
}

const String MaterialAsset::GetPathToShaderAsset() const
{
	return m_ShaderAsset ? m_ShaderAsset->GetPathToAsset() : "";
}
