#pragma once

#include <Core/includes/Level.h>
//#include <../Editor/includes/Test/TestLight.h>
//#include <../Editor/includes/Test/TestLightActor.h>

#include <Core/includes/World.h>
#include <../EditorEngine/Editor/includes/Test/TestController.h>
#include <../EditorEngine/Editor/includes/Test/TestLight.h>
#include <../EditorEngine/Editor/includes/Test/TestQuad.h>
#include <../EditorEngine/Editor/includes/Test/TestLightActor.h>
//#include <Editor/includes/Test/TestLight.h>
//#include <Editor/includes/Test/TestLightActor.h>
//#include <Editor/includes/Test/TestQuad.h>
#include <TestEditorLevel.generated.h>

class MyController;
class Quad;
class Light;
class LightActor;



RCLASS()
class FirstLevel : public CoreEngine::Level
{
	GENERATED_BODY()

public:

	FirstLevel(const CoreEngine::InitializeObject& Object) : Level(Object)
	{
	}
private:

	virtual void ActorInitialize() override
	{
		auto* controller = GetWorld()->SpawnActor<class MyController>(nullptr);
		auto* pawn = GetWorld()->SpawnActor<class Quad>(controller);
		auto* acc = GetWorld()->SpawnActor<class Light>(controller);
		auto* ac = GetWorld()->SpawnActor<class LightActor>(controller);
		controller->Possess(pawn);

		Level::ActorInitialize();
	}
};