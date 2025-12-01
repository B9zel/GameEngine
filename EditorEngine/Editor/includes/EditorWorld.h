#pragma once
#include <Core/includes/World.h>
#include <Core/includes/Memory/SaveManager.h>
#include <EditorWorld.generated.h>

namespace Editor
{
	class EditorEngine;
}


RCLASS()
class EditorWorld : public CoreEngine::World
{
	GENERATED_BODY()

public:

	EditorWorld(const CoreEngine::InitializeObject& Initializer);

protected:

	virtual void UpdateGroup() override;

private:

	Editor::EditorEngine* EditEngine;
};
