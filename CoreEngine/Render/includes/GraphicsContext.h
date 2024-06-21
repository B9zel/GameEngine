#pragma once
#include <Core/includes/Base.h>



namespace CoreEngine
{
	namespace Render
	{
		class GraphicsContext
		{
		public:

			virtual void Init() = 0;
			virtual void SwapBuffer() = 0;

			static UniquePtr<GraphicsContext> CreateContext(void* window);
		};
	}

}