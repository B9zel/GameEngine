#include <Core/includes/ShaderLibrary.h>

namespace CoreEngine
{
	
	void ShaderLibrary::AddShader(const String& name, Render::Shader* shader)
	{
		if (IsValidShader(name))
		{
			EG_LOG(CORE, ELevelLog::WARNING, "The key \"{0}\" alredy exists");
		}
		m_shaders[name] = shader;
	}

	bool ShaderLibrary::RemoveShader(const String& name)
	{			
		return static_cast<bool>(m_shaders.erase(name));
	}

	bool ShaderLibrary::IsValidShader(const String& name)
	{
		return m_shaders.count(name) > 0;
	}
	Render::Shader* ShaderLibrary::GetShader(const String& name)
	{
		return m_shaders[name];
	}
}