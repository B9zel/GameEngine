#pragma once
#include <Render/includes/MaterialAsset.h>
#include <Editor/includes/EditorDrawingInterface.h>
#include <EditorMaterialAsset.generated.h>

RCLASS()
class EditorMaterialAsset : public MaterialAsset, public Editor::IEditorDrawingInterface
{
	GENERATED_BODY()

public:

	EditorMaterialAsset(const CoreEngine::InitializeObject& Initilize);

public:

	virtual void DrawElements() override;

private:

	//DArray<UniquePtr<CoreEngine::Render::BaseMaterialProperty>> m_MaterialProperties;
};
