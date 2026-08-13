#pragma once
#include <Core/includes/MeshAsset.h>
#include <Editor/includes/EditorDrawingInterface.h>
#include <EditorMeshAsset.generated.h>

RCLASS()
class EditorMeshAsset : public MeshAsset, public Editor::IEditorDrawingInterface
{
	GENERATED_BODY()

public:

	EditorMeshAsset(const CoreEngine::InitializeObject& Initilize);

public:

	virtual void DrawElements() override;
};
