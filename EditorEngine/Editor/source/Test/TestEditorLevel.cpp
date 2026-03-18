#include <Editor/includes/Test/TestEditorLevel.h>
#include <Editor/includes/Test/TestController.h>
#include <Editor/includes/Test/TestLight.h>
#include <Editor/includes/Test/TestLightActor.h>
#include <Editor/includes/Test/TestQuad.h>

void FirstLevel::ActorInitialize()
{
	
	//auto* controller = GetWorld()->SpawnActor<class MyController>(nullptr);
	auto* pawn = GetWorld()->SpawnActor<class Quad>(nullptr);
	auto* acc = GetWorld()->SpawnActor<class Light>(nullptr);
	auto* ac = GetWorld()->SpawnActor<class LightActor>(nullptr);
	//controller->Possess(pawn);

	Level::ActorInitialize();
	
}
