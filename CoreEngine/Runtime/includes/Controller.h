#pragma once
#include <Runtime/includes/Actor.h>
#include <Controller.generated.h>

class Pawn;

RCLASS()
class Controller : public Actor
{
	GENERATED_BODY()

public:

	Controller(const CoreEngine::InitializeObject& Object);

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
