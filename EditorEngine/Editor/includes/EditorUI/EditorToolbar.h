#pragma once
#include <Editor/includes/EditorUI/BaseEditorPanel.h>
#include <Platform/Renderer/OpenGL/include/OpenGLTexture.h>

namespace Editor
{
	class EditorToolbar : public BaseEditorPanel
	{
	public:

		EditorToolbar();

	public:

		virtual void Draw() override;

	private:

		SharedPtr<CoreEngine::Render::Texture2D> StopTexture;
		SharedPtr<CoreEngine::Render::Texture2D> PlayTexture;
	};

}
