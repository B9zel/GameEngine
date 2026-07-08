#include <Editor/includes/EditorWorld.h>
#include <Editor/includes/EditorEngine.h>
#include <Editor/includes/EditorApplication.h>
#include <Editor/includes/EditorViewportClient.h>

EditorWorld::EditorWorld(const CoreEngine::InitializeObject& Initializer) : World(Initializer)
{
	EditEngine = dynamic_cast<EditorEngine*>(EditorEngine::Get());
}

FVector EditorWorld::GetControllerLocation() const
{
	return EditEngine->GetViewpoertClient()->GetLocation();
}

void EditorWorld::UpdateWorld()
{
	if (EditEngine->GetCurrentStateWorld() == EStateWorld::Play)
	{
	}
	World::UpdateWorld();

	//EG_LOG(CoreEngine::CORE, ELevelLog::INFO, 1.0f / GetWorldDeltaTime());
}
