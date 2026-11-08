#pragma once
#include <Render/includes/ShaderAsset.h>
#include <Editor/includes/EditorDrawingInterface.h>
#include <TextEditor.h>
#include <EditorShaderAsset.generated.h>

RCLASS()
class EditorShaderAsset : public ShaderAsset, public Editor::IEditorDrawingInterface
{
	GENERATED_BODY()

public:

	EditorShaderAsset(const CoreEngine::InitializeObject& Initialize);

public:

	virtual void DrawElements() override;
	virtual void OnDeserialize(CoreEngine::SerializeAchive& Achive) override;
	void OpenEditor();
	bool DrawEditor();

private:

	void ApplyChange();
	void InitializeEditors();

private:

	SourceShader ChangedCustomShader;

	TextEditor VertexEditor;
	TextEditor FragmentEditor;
	bool m_EditorsInitialized{false};
	bool m_IsEditorOpen{false};
	bool m_FocusEditor{false};
	float m_CodeFontSize{13.0f};
};
