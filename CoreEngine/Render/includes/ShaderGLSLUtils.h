#pragma once
#include <Core/includes/Base.h>
#include <Render/includes/UniformType.h>

namespace CoreEngine::Render::GLSL
{
	struct UniformFileInfo
	{
		EUniformType Type;
	};

	bool IsInComment(const String& str, const size_t PosTarget);
	void ParseShader(const String& shader, HashTableMap<String, UniformFileInfo>& outUniforms);
} // namespace CoreEngine::Render::GLSL
