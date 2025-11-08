#pragma once
#include <Runtime/includes/PlayerController.h>
#include <Core/includes/Engine.h>
#include <Core/includes/InputDevice.h>
#include <Math/includes/Math.h>
#include <Runtime/includes/InputComponent.h>
#include <TestController.generated.h>



RCLASS()
class MyController : public CoreEngine::Runtime::PlayerController
{
	GENERATED_BODY()


public:

	MyController(const CoreEngine::InitializeObject& Object) : PlayerController(Object)
	{
		SetActorRotation(FVector(0, 0, 0));
		LastMousePos = FVector2(0.0f, 0.0f);
	}

protected:


	virtual void SetupInputComponent() override
	{
		PlayerController::SetupInputComponent();

		inputComponent->BindAxis(GLFW_KEY_W, 1, &MyController::MoveForward, this);
		inputComponent->BindAxis(GLFW_KEY_S, -1, &MyController::MoveForward, this);
		inputComponent->BindAxis(GLFW_KEY_A, 1, &MyController::MoveLeft, this);
		inputComponent->BindAxis(GLFW_KEY_D, -1, &MyController::MoveLeft, this);
		inputComponent->BindAxis(GLFW_KEY_E, 1, &MyController::MoveUp, this);
		inputComponent->BindAxis(GLFW_KEY_Q, -1, &MyController::MoveUp, this);
		inputComponent->BindAction(GLFW_MOUSE_BUTTON_RIGHT, CoreEngine::EActionType::PRESSED, &MyController::RightPress, this);
		inputComponent->BindAction(GLFW_MOUSE_BUTTON_RIGHT, CoreEngine::EActionType::RELEASED, &MyController::RightRelease, this);
		inputComponent->BindMouseMotionAxis(&MyController::MoveRight, this);
	}

	void RightPress()
	{
		CurrentMouseLoc = CoreEngine::Engine::Get()->GetInputDevice()->GetMousePos();
		IsLooking = true;
	}

	void RightRelease()
	{

		IsLooking = false;
	}

	void MoveRight(double axisX, double axisY)
	{
		if (!IsLooking) return;
		if (LastMousePos.x == 0.0f && LastMousePos.y == 0.0f)
		{
			LastMousePos = CoreEngine::Engine::Get()->GetInputDevice()->GetMousePos();
			return;
		}
		DVector2 NowPos = CoreEngine::Engine::Get()->GetInputDevice()->GetMousePos();
		double DeltaX = Math::Clamp(LastMousePos.x - NowPos.x, -360, 360);
		double DeltaY = Math::Clamp(LastMousePos.y - NowPos.y, -360, 360);

		if (DeltaX != 0.0f)
		{

			SetActorRotation(GetActorRotation() + FVector(0.0, (-DeltaX * 0.1), 0.0));
		}
		if (DeltaY != 0.0f)
		{

			SetActorRotation(GetActorRotation() + FVector((DeltaY * 0.1), 0.0f, 0.0f));
		}
		LastMousePos = CoreEngine::Engine::Get()->GetInputDevice()->GetMousePos();
		CoreEngine::Engine::Get()->GetInputDevice()->SetMousePos(LastMousePos);

	}

	void MoveLeft(float axis)
	{
		if (axis == 1)
		{
			AddActorLocation(GetActorRightVector() * 0.01);
		}
		if (axis == -1)
		{
			AddActorLocation(-GetActorRightVector() * 0.01);
		}
	}
	void MoveUp(float axis)
	{
		AddActorLocation(FVector(0, 0.01 * axis, 0.0f));
	}

	void MoveForward(float axis)
	{
		if (axis == 1)
		{
			AddActorLocation(GetActorForwardVector() * 0.01);
		}
		if (axis == -1)
		{
			AddActorLocation(-GetActorForwardVector() * 0.01);
		}
	}

	virtual void Update(float DeltaTime) override
	{
		CoreEngine::Runtime::PlayerController::Update(DeltaTime);

		if (IsLooking)
		{

		}

	}


private:

	bool IsLooking{ false };
	DVector2 LastMousePos;
	DVector2 CurrentMouseLoc;
};
