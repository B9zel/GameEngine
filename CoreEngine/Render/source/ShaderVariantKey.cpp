#include <Render/includes/ShaderVariantKey.h>

size_t CoreEngine::Render::ShaderVariantKeyHasher::operator()(const ShaderVariantKey& Key) const
{
	size_t Hash = std::hash<uint64>()(Key.m_Hash);

	Hash ^= std::hash<EShaderRenderType>()(Key.ModeRender) << 1;
	return Hash;
}
