#include <Editor/includes/EditorWorld.h>
#include <Editor/includes/EditorEngine.h>
#include <Editor/includes/EditorApplication.h>
#include <Editor/includes/EditorViewportClient.h>


EditorWorld::EditorWorld(const CoreEngine::InitializeObject& Initializer) : CoreEngine::World(Initializer)
{
	EditEngine = dynamic_cast<Editor::EditorEngine*>(Editor::EditorEngine::Get());
}

FVector EditorWorld::GetControllerLocation() const
{
	return EditEngine->GetViewpoertClient()->GetLocation();
}

void EditorWorld::UpdateWorld()
{
	if (EditEngine->GetCurrentStateWorld() == Editor::EStateWorld::Play)
	{
		CoreEngine::World::UpdateWorld();
	}
}

