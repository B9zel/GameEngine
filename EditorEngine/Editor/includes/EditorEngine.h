#pragma once
#include <Core/includes/Engine.h>
#include <Core/includes/InputDevice.h>
#include <Render/includes/RenderDevice.h>

#include <Render/includes/Framebuffer.h>

#include <EditorEngine.generated.h>

class World;

namespace Editor
{
	class EditorViewportClient;
	class EditorViewport;
	class EditorMenuBar;
	class EditorToolbar;
	class SceneHierarhy;
	class EditorDetails;
	class BaseEditorPanel;
	class ContentBrowser;
} // namespace Editor

enum class EStateWorld : uint8
{
	Edit = 0,
	Play
};

RCLASS()
class EditorEngine : public Engine
{
	GENERATED_BODY()

public:

	using ThisClass = EditorEngine;

public:

	EditorEngine(const CoreEngine::InitializeObject& Initilize);

public:

	virtual void Update() override;
	void SetSelectedObject(Object* NewSelected);
	Object* GetSelectedObject() const;
	Editor::EditorViewportClient* GetViewpoertClient() const;

	EStateWorld GetCurrentStateWorld() const;
	void SetCurrentStateWorld(EStateWorld NewState);
	virtual void PostInitialize() override;

protected:

	void RenderEditor();
	virtual World* CreateWorld() const override;

private:

	float f;
	String buf;
	float my_color[4];
	bool my_tool_active{true};

	SharedPtr<CoreEngine::Render::Framebuffer> FrameBuffer;
	DArray<SharedPtr<Editor::BaseEditorPanel>> EditorWidgets;

	SharedPtr<Editor::EditorViewport> Viewport;
	SharedPtr<Editor::SceneHierarhy> SceneHier;
	SharedPtr<Editor::EditorDetails> DetailsPanel;
	SharedPtr<Editor::EditorMenuBar> MenuBar;
	SharedPtr<Editor::EditorToolbar> Toolbar;
	SharedPtr<Editor::ContentBrowser> ContentBrowser;

	EStateWorld CurretnStateWorld{EStateWorld::Edit};

	UniquePtr<Editor::EditorViewportClient> m_ViewportCamera;

	Object* SelectedObject{nullptr};
};
