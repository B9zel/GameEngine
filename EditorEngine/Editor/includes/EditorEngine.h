#pragma once
#include <Core/includes/Engine.h>
#include <Core/includes/InputDevice.h>
#include <Editor/includes/EditorViewport.h>
#include <Render/includes/Framebuffer.h>
#include <Editor/includes/SceneHierarhy.h>
#include <Editor/includes/EditorDetails.h>


namespace Editor
{
	class EditorViewportClient;

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

	protected:

		void RenderEditor();

	private:

		float f;
		String buf;
		float my_color[4];
		bool my_tool_active{ true };

		SharedPtr<CoreEngine::Render::Framebuffer> FrameBuffer;
		DArray<SharedPtr<BaseEdtiorPanel>> EditorWidgets;

		SharedPtr<EditorViewport> Viewport;
		SharedPtr<SceneHierarhy> SceneHier;
		SharedPtr<EditorDetails> DetailsPanel;

		UniquePtr<EditorViewportClient> m_ViewportCamera;

		CoreEngine::Runtime::Object* SelectedObject{ nullptr };
	};
}