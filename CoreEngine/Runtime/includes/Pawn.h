#pragma once
#include <Runtime/includes/Actor.h>


namespace CoreEngine
{
	namespace Runtime
	{	
		class Controller;
		class PlayerController;
		class InputComponent;

		class Pawn : public Actor
		{
		public:

			Pawn();

		public:

			Controller* GetOwningController() const;
			virtual void PossessedBy(Controller* NewController);
			virtual void Update(float deltaTime) override;

			void PawnRestart();

		protected:

			virtual void SetupInputPlayerController(InputComponent* inputController);
		
		protected:

			Controller* OwningController;
			ObjectPtr<InputComponent> inputComponent;

			friend PlayerController;
		};
	}
}