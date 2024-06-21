#pragma once
#include <Render/includes/RendererAPI.h>

namespace CoreEngine
{
	namespace Render
	{
		class Render
		{
		public:

			Render();

			static UniquePtr<Render> Create();

		private:

			UniquePtr<RendererAPI> m_renderAPI;

			static bool isInit;
		};
	}
}