#pragma once

class EditorEngine;

namespace Editor
{

	class BaseEditorPanel
	{
	public:

		BaseEditorPanel() = default;
		void SetOwnerEditor(EditorEngine* OwnerEditor);
		virtual void Draw() = 0;
		virtual void OnConstruct() = 0;

	protected:

		EditorEngine* OwnerEditor;
	};
} // namespace Editor
