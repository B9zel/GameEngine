#include <Editor/includes/EditorWorld.h>
#include <Editor/includes/EditorEngine.h>
#include <Editor/includes/EditorApplication.h>

EditorWorld::EditorWorld(const CoreEngine::InitializeObject& Initializer) : CoreEngine::World(Initializer)
{
	EditEngine = dynamic_cast<Editor::EditorEngine*>(Editor::EditorEngine::Get());
}

void EditorWorld::UpdateGroup()
{
	if (EditEngine->GetCurrentStateWorld() == Editor::EStateWorld::Play)
	{
		CoreEngine::World::UpdateGroup();
	}
}

