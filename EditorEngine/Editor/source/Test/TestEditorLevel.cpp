#include <Editor/includes/Test/TestEditorLevel.h>
#include <Editor/includes/Test/TestController.h>
#include <Editor/includes/Test/TestLight.h>
#include <Editor/includes/Test/TestLightActor.h>
#include <Editor/includes/Test/TestQuad.h>
#include <Core/includes/StringUtil.h>

void FirstLevel::ActorInitialize()
{

	// auto* controller = GetWorld()->SpawnActor<class MyController>(nullptr);
	auto* pawn = GetWorld()->SpawnActor<class Quad>(nullptr, FTransform(FVector(), FVector(), FVector(1.0f)));

	auto* acc = GetWorld()->SpawnActor<class Light>(nullptr, FTransform(FVector(), FVector(), FVector(1.0f)));

	auto* ac = GetWorld()->SpawnActor<class LightActor>(nullptr, FTransform(FVector(), FVector(), FVector(1.0f)));
	//  controller->Possess(pawn);

	Level::ActorInitialize();
}
