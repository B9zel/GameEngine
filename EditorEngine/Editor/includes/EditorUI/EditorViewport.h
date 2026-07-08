#pragma once
#include <Core/includes/Base.h>
#include <Editor/includes/EditorUI/BaseEditorPanel.h>
#include <Math/includes/Matrix.h>
#include <imgui/imgui.h>

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
		void UpdateTypeCursor();
		virtual void OnConstruct() override;

		void SetFrameBuffer(const SharedPtr<CoreEngine::Render::Framebuffer>& Buffer);
		bool GetIsFocused() const;

		void OnActiveMoveCamera(bool IsActive);

	private:

		SceneComponent* GetSceneComponentFromSelected() const;
		FMatrix4x4 GetMatrixOfComponent(SceneComponent* Component) const;

		void BindMoveCamera(const bool IsMove);

	private:

		CoreEngine::Render::Framebuffer* FrameBuffer;

		int32 m_GuizmoOpiration;
		bool m_CanChangeOpirations{true};
		bool m_IsMoveCamera{false};
		bool m_IsMoveCameraLastFrame{false};

		ImVec2 PosCursorBeforeMove;

		bool m_IsFocusedViewport;
		uint32 Width{0}, Height{0};
	};
} // namespace Editor
