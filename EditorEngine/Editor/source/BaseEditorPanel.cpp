#include "Editor/includes/BaseEditorPanel.h"
#include <Editor/includes/EditorEngine.h>

namespace Editor
{
	void BaseEdtiorPanel::SetOwnerEditor(EditorEngine* OwnerEditor)
	{
		this->OwnerEditor = OwnerEditor;
	}
}