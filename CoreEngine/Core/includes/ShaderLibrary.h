#pragma once
#include <Core/includes/Base.h>
#include <Render/includes/Shader.h>


namespace CoreEngine
{
	class ShaderLibrary
	{
	public:
		ShaderLibrary() = default;

		void AddShader(const String& name, Render::Shader* shader);
		bool RemoveShader(const String& name);
		bool IsValidShader(const String& name);

		Render::Shader* GetShader(const String& name);

	private:

		HashTableMap<String, Render::Shader*> m_shaders;
	};
}