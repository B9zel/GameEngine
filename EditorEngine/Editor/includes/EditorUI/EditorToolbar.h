#pragma once
#include <Editor/includes/EditorUI/BaseEditorPanel.h>
#include <Platform/Renderer/OpenGL/include/OpenGLTexture.h>

class CoreEngine::AssetManager;
class CoreEngine::Render::Texture2D;

namespace Editor
{

	class EditorToolbar : public BaseEditorPanel
	{
	public:

		EditorToolbar();

	public:

		virtual void Draw() override;
		virtual void OnConstruct() override;

	private:

		CoreEngine::Render::Texture2D* StopTexture;
		CoreEngine::Render::Texture2D* PlayTexture;

		CoreEngine::AssetManager* AssetManager;
		CoreEngine::Render::RenderDevice* DeviceRender;
	};

} // namespace Editor
