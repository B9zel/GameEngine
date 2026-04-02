#pragma once
#include <Editor/includes/EditorUI/BaseEditorPanel.h>
#include <Core/includes/Base.h>

namespace Editor
{
	class EditorMenuBar : public BaseEditorPanel
	{
	public:

		virtual void Draw() override;
		virtual void OnConstruct() override;

	private:

		String SaveFileDialogeMenu(const char* Filter);
		String OpenFileDialoge(const char* Filter);
	};
} // namespace Editor
