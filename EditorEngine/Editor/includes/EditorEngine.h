#pragma once
#include <Core/includes/Engine.h>
#include <Core/includes/InputDevice.h>
#include <Render/includes/RenderDevice.h>

#include <Render/includes/Framebuffer.h>

#include <EditorEngine.generated.h>

namespace CoreEngine
{
	class World;
}
namespace Editor
{
	class EditorViewportClient;
	class EditorViewport;
	class EditorMenuBar;
	class EditorToolbar;
	class SceneHierarhy;
	class EditorDetails;
	class BaseEditorPanel;
} // namespace Editor

enum class EStateWorld : uint8
{
	Edit = 0,
	Play
};

RCLASS()
class EditorEngine : public CoreEngine::Engine
{
	GENERATED_BODY()

public:

	using ThisClass = EditorEngine;

public:

	EditorEngine(const CoreEngine::InitializeObject& Initilize);

public:

	virtual void Update() override;
	void SetSelectedObject(CoreEngine::Runtime::Object* NewSelected);
	CoreEngine::Runtime::Object* GetSelectedObject() const;
	Editor::EditorViewportClient* GetViewpoertClient() const;

	EStateWorld GetCurrentStateWorld() const;
	void SetCurrentStateWorld(EStateWorld NewState);
	virtual void PostInitialize() override;

protected:

	void RenderEditor();
	virtual CoreEngine::World* CreateWorld() const override;

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

	EStateWorld CurretnStateWorld{EStateWorld::Edit};

	UniquePtr<Editor::EditorViewportClient> m_ViewportCamera;

	CoreEngine::Runtime::Object* SelectedObject{nullptr};
};
