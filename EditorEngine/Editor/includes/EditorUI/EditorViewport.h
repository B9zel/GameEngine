#pragma once
#include <Core/includes/Base.h>
#include <Editor/includes/EditorUI/BaseEditorPanel.h>
#include <Math/includes/Matrix.h>


namespace CoreEngine::Runtime
{
	class SceneComponent;
}

namespace CoreEngine::Render
{
	class Framebuffer;
}


namespace Editor
{
	class EditorViewport : public BaseEditorPanel
	{
	public:

		EditorViewport();

	public:

		virtual void Draw() override;

		void SetFrameBuffer(const SharedPtr<CoreEngine::Render::Framebuffer>& Buffer);
		bool GetIsFocused() const;

		void OnActiveMoveCamera(bool IsActive);
	private:

		CoreEngine::Runtime::SceneComponent* GetSceneComponentFromSelected() const;
		FMatrix4x4 GetMatrixOfComponent(CoreEngine::Runtime::SceneComponent* Component) const;

	private:

		SharedPtr<CoreEngine::Render::Framebuffer> FrameBuffer;

		int32 m_GuizmoOpiration;
		bool m_CanChangeOpirations{true};

		bool m_IsFocusedViewport;
		uint32 Width{0}, Height{0};
	};
}