#include "Editor/includes/EditorUI/BaseEditorPanel.h"
#include <Editor/includes/EditorEngine.h>

namespace Editor
{
	void BaseEditorPanel::SetOwnerEditor(EditorEngine* OwnerEditor)
	{
		this->OwnerEditor = OwnerEditor;
	}
}