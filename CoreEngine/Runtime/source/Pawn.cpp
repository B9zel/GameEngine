#include <Runtime/includes/Pawn.h>
#include <Runtime/includes/InputComponent.h>
#include <Runtime/includes/Controller.h>


namespace CoreEngine
{
	namespace Runtime
	{
		Pawn::Pawn()
		{
			OwningController = nullptr;
		}

		Controller* Pawn::GetOwningController() const
		{
			return OwningController;
		}

		void Pawn::PossessedBy(Controller* NewController)
		{
			SetOwner(NewController);
			OwningController = NewController;

		}

		void Pawn::Update(float deltaTime)
		{
			Actor::Update(deltaTime);

		}

		void Pawn::PawnRestart()
		{
			if (!inputComponent)
			{
				inputComponent = CreateObjectWithInit<InputComponent>(this);
				if (inputComponent)
				{
					inputComponent->RegisteredComponent();
					
					SetupInputPlayerController(inputComponent.Get());
				}
			}
		}

		void Pawn::SetupInputPlayerController(InputComponent* inputController)
		{
			
		}
	}
}
