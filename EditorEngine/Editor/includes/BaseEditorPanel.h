#pragma once

namespace Editor
{
	class EditorEngine;

	class BaseEdtiorPanel
	{
	public:

		BaseEdtiorPanel() = default;
		void SetOwnerEditor(Editor::EditorEngine* OwnerEditor);
		virtual void Draw() = 0;

	protected:

		EditorEngine* OwnerEditor;
	};
}