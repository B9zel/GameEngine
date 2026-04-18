#pragma once
#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#include <Core/includes/Platform.h>
#include <Render/includes/RenderHandle.h>
#include <Core/includes/Log.h>

namespace CoreEngine::Render::RHI
{

	struct BufferHandle : public RenderHandle
	{
	};

	struct TextureHandle : public RenderHandle
	{
	};

	struct ShaderHandle : public RenderHandle
	{
	};

	struct HandleVAO : public RenderHandle
	{
	};

} // namespace CoreEngine::Render::RHI
