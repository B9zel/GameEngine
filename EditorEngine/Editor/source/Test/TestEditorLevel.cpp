#include <Editor/includes/Test/TestEditorLevel.h>
#include <Editor/includes/Test/TestController.h>
#include <Editor/includes/Test/TestLight.h>
#include <Editor/includes/Test/TestLightActor.h>
#include <Editor/includes/Test/TestQuad.h>

void FirstLevel::ActorInitialize()
{
	
	auto* controller = GetWorld()->SpawnActor<class MyController>(nullptr);
	auto* pawn = GetWorld()->SpawnActor<class Quad>(controller);
	auto* acc = GetWorld()->SpawnActor<class Light>(controller);
	auto* ac = GetWorld()->SpawnActor<class LightActor>(controller);
	controller->Possess(pawn);

	Level::ActorInitialize();
	
}
