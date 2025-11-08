#pragma once
#include <Core/includes/Base.h>

namespace CoreEngine::Render
{
	class Framebuffer;
}


namespace Editor
{
	class EditorViewport
	{
	public:

		EditorViewport();

	public:

		void DrawViewport(SharedPtr<CoreEngine::Render::Framebuffer>& FrameBuffer);

	private:

		bool m_IsFocusedViewport;
		uint32 Width{0}, Height{0};
	};
}