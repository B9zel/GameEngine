#pragma once
#include <Runtime/includes/Actor.h>
#include <Runtime/includes/InputComponent.h>
#include <Pawn.generated.h>

namespace CoreEngine
{
	namespace Runtime
	{
		class Controller;
		class PlayerController;
		class InputComponent;

		RCLASS();
		class Pawn : public Actor
		{
			GENERATED_BODY()

		public:

			Pawn(const InitializeObject& Object);

		public:

			Controller* GetOwningController() const;
			virtual void PossessedBy(Controller* NewController);
			virtual void Update(float deltaTime) override;

			void PawnRestart();

		protected:

			virtual void SetupInputPlayerController(InputComponent* inputController);

		protected:

			Controller* OwningController;
			RPROPERTY();
			InputComponent* inputComponent;

			friend PlayerController;
		};

	}
}