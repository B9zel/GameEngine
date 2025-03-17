#include <Runtime/includes/Controller.h>
#include <Runtime/includes/Pawn.h>


namespace CoreEngine
{
	namespace Runtime
	{
		Pawn* Controller::GetControlledPawn() const
		{
			return ControlledPawn;
		}

		void Controller::SetControlledPawn(Pawn* NewPawn)
		{
			ControlledPawn = NewPawn;
		}

		void Controller::Possess(Pawn* pawn)
		{
			Pawn* currentPawn = GetControlledPawn();
			
			OnPossess(pawn);
		}

		void Controller::OnPossess(Pawn* NewPawn)
		{
			if (NewPawn && GetControlledPawn())
			{
				UnPossess();
			}

			if (NewPawn == nullptr) return;

			if (NewPawn->GetOwningController())
			{
				NewPawn->GetOwningController()->UnPossess();
			}


			NewPawn->PossessedBy(this);
			SetControlledPawn(NewPawn);
		}

		void Controller::UnPossess()
		{
			OnUnPossess();
			SetControlledPawn(nullptr);
		}

		void Controller::OnUnPossess()
		{

		}
	}
}