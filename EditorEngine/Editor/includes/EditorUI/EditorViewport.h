#pragma once
#include <Core/includes/Base.h>
#include <Editor/includes/EditorUI/BaseEditorPanel.h>
#include <Math/includes/Matrix.h>

class SceneComponent;

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
		virtual void OnConstruct() override;

		void SetFrameBuffer(const SharedPtr<CoreEngine::Render::Framebuffer>& Buffer);
		bool GetIsFocused() const;

		void OnActiveMoveCamera(bool IsActive);

	private:

		SceneComponent* GetSceneComponentFromSelected() const;
		FMatrix4x4 GetMatrixOfComponent(SceneComponent* Component) const;

	private:

		CoreEngine::Render::Framebuffer* FrameBuffer;

		int32 m_GuizmoOpiration;
		bool m_CanChangeOpirations{true};

		bool m_IsFocusedViewport;
		uint32 Width{0}, Height{0};
	};
} // namespace Editor
