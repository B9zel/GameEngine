#pragma once
#include <Core/includes/Engine.h>
#include <Core/includes/InputDevice.h>
#include <Editor/includes/EditorUI/EditorViewport.h>
#include <Render/includes/Framebuffer.h>
#include <Editor/includes/EditorUI/SceneHierarhy.h>
#include <Editor/includes/EditorUI/EditorDetails.h>




namespace Editor
{
	class EditorViewportClient;
	class EditorMenuBar;

	class EditorEngine : public CoreEngine::Engine
	{
	public:

		using ThisClass = EditorEngine;

	public:

		EditorEngine();

	public:

		virtual void Update() override;
		void SetSelectedObject(CoreEngine::Runtime::Object* NewSelected);
		CoreEngine::Runtime::Object* GetSelectedObject() const;
		EditorViewportClient* GetViewpoertClient() const;

	protected:

		void RenderEditor();

	private:

		float f;
		String buf;
		float my_color[4];
		bool my_tool_active{ true };

		SharedPtr<CoreEngine::Render::Framebuffer> FrameBuffer;
		DArray<SharedPtr<BaseEditorPanel>> EditorWidgets;

		SharedPtr<EditorViewport> Viewport;
		SharedPtr<SceneHierarhy> SceneHier;
		SharedPtr<EditorDetails> DetailsPanel;
		SharedPtr<EditorMenuBar> MenuBar;

		UniquePtr<EditorViewportClient> m_ViewportCamera;

		CoreEngine::Runtime::Object* SelectedObject{ nullptr };
	};
}