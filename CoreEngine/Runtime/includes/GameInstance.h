#pragma once
#include <Runtime/CoreObject/Include/Object.h>
#include <GameInstance.generated.h>


namespace CoreEngine
{
	namespace Runtime
	{
		RCLASS()
			class GameInstance : public CoreEngine::Runtime::Object
		{
			GENERATED_BODY()

		public: 

			GameInstance(const InitializeObject& Initilize) : Object(Initilize) {}

		};


	}
}