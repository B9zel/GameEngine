#pragma once

#include <Core/includes/Level.h>
//#include <../Editor/includes/Test/TestLight.h>
//#include <../Editor/includes/Test/TestLightActor.h>

#include <Core/includes/World.h>
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

	virtual void ActorInitialize() override;
	
};