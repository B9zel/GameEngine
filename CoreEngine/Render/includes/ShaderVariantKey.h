#pragma once
#include <Render/includes/Enums/ShaderRenderType.h>
#include <Core/includes/Platform.h>
#include <xhash>

namespace CoreEngine::Render
{
	struct ShaderVariantKey
	{
	public:

		bool operator==(const ShaderVariantKey& Other) const
		{
			return m_Hash == Other.m_Hash && ModeRender == Other.ModeRender;
		}

	public:

		uint64 m_Hash;
		EShaderRenderType ModeRender;
	};

	struct ShaderVariantKeyHasher
	{
		size_t operator()(const ShaderVariantKey& Key) const;
	};

} // namespace CoreEngine::Render
