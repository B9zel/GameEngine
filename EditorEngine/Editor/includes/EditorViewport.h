#pragma once
#include <Core/includes/Base.h>
#include <Editor/includes/BaseEditorPanel.h>

namespace CoreEngine::Render
{
	class Framebuffer;
}


namespace Editor
{
	class EditorViewport : public BaseEdtiorPanel
	{
	public:

		EditorViewport();

	public:

		virtual void Draw() override;

		void SetFrameBuffer(const SharedPtr<CoreEngine::Render::Framebuffer>& Buffer);
		bool GetIsFocused() const;

	private:

		SharedPtr<CoreEngine::Render::Framebuffer> FrameBuffer;

		bool m_IsFocusedViewport;
		uint32 Width{0}, Height{0};
	};
}