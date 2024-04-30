#pragma once
#include <Core/includes/Base.h>
#include <Render/includes/RendererAPI.h>


namespace CoreEngine
{
	class GraphicsContext
	{
	public:

		virtual void Init() = 0;
		virtual void SwapBuffer() = 0;

		static UniquePtr<GraphicsContext> CreateContext();
	};


}