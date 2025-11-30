#pragma once

namespace Editor
{
	class EditorEngine;

	class BaseEditorPanel
	{
	public:

		BaseEditorPanel() = default;
		void SetOwnerEditor(Editor::EditorEngine* OwnerEditor);
		virtual void Draw() = 0;

	protected:

		EditorEngine* OwnerEditor;
	};
}