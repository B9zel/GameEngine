#pragma once 
#include <Core/includes/Base.h>

namespace CoreEngine
{
	class RendererAPI
	{
	public:
		enum class API : short
		{
			None = 0,
			OpenGL
		};
	public:

		static API GetAPI();

		static UniquePtr<RendererAPI> CreateAPI();

	private:
		static API m_Api;
	};
}