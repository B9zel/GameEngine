#pragma once
#include <Render/includes/ShaderAsset.h>
#include <Editor/includes/EditorDrawingInterface.h>
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

private:

	void ApplyChange();

private:

	SourceShader ChangedCustomShader;

	const float m_PercenBordertCapacity = 0.9f;
	const float m_RatioMultySizeShaderStr = 1.5f;
};
