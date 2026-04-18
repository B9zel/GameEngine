#pragma once
#include <Runtime/CoreObject/Include/Object.h>
#include <GameInstance.generated.h>

RCLASS()
class GameInstance : public Object
{
	GENERATED_BODY()

public:

	GameInstance(const CoreEngine::InitializeObject& Initilize) : Object(Initilize)
	{
	}
};
