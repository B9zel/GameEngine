#pragma once
#include <Runtime/includes/Actor.h>



namespace CoreEngine
{
	namespace Runtime
	{
		class Pawn;

		class Controller : public Actor
		{
		public:

			Pawn* GetControlledPawn() const;
			void SetControlledPawn(Pawn* NewPawn);

			virtual void Possess(Pawn* pawn);
			virtual void UnPossess();

		protected:

			virtual void OnPossess(Pawn* NewPawn);
			virtual void OnUnPossess();

		protected:

			Pawn* ControlledPawn;
		};
	}
}