#include <Runtime/includes/PlayerController.h>

#include <Runtime/includes/Pawn.h>
#include <Core/includes/Base.h>



namespace CoreEngine
{
	namespace Runtime
	{
		PlayerController::PlayerController(const InitializeObject& Object) : Controller(Object)
		{
			canTransferEvent = true;
		}

		void PlayerController::Update(float deltaTime)
		{
			Controller::Update(deltaTime);

			if (cameraManager)
			{
				cameraManager->Update(deltaTime);
			}
		}
		void PlayerController::OnRegistered()
		{
			Controller::OnRegistered();

			playerInput = MakeUniquePtr<PlayerInput>();
			playerInput->Register();

			inputComponent = CreateSubObject<InputComponent>();
			inputComponent->RegisteredComponent();
			inputComponent->TakeEvent.AddBind(&PlayerController::OnTakeEvent, this);
			inputComponent->TakeProcessedEvent.AddBind(&PlayerController::OnTakeProcessedEvent, this);

			cameraManager = CreateObject<CameraManager>(this);
			cameraManager->Registered();
			cameraManager->SetOwningPlayerController(this);
			cameraManager->SetOwner(this);


			updateInput.SetUpdateMethod(static_cast<void(Actor::*)(float)>(&PlayerController::ProcessInput), this);
			GetWorld()->GetUpdateManager()->AddFunction(&updateInput);
		}


		void PlayerController::OnPossess(Pawn* NewPawn)
		{
			Controller::OnPossess(NewPawn);

			SetupInputComponent();

			GetControlledPawn()->PawnRestart();
		}

		void PlayerController::SetupInputComponent()
		{
			if (!inputComponent)
			{
				inputComponent = CreateObject<InputComponent>(this);
				inputComponent->RegisteredComponent();
			}
		}
		void PlayerController::OnTakeEvent(Event& event)
		{
			if (GetControlledPawn() && GetControlledPawn()->inputComponent)
			{
				//if (!inputComponent->HasProcessingCurrentFrame())
				//{
				///	GetControlledPawn()->inputComponent->ExecuteEvent(event);
				//}
			}
		}
		void PlayerController::OnTakeProcessedEvent(Event& event)
		{
			canTransferEvent = false;
		}
		void PlayerController::ProcessInput(float deltaTime)
		{
			Queue<Event*>* events = (playerInput->GetReverseQueueEvents());

			auto& InputControlledPawn = GetControlledPawn()->inputComponent;
			int64 size = events->Size();

			for (int64 i = 0; i < size; i++)
			{
				inputComponent->ProcessUpdateAxis(events->Front());
				inputComponent->ProcessUpdateAction(events->Front());
				if (InputControlledPawn)
				{
					InputControlledPawn->ProcessUpdateAxis(events->Front());
					InputControlledPawn->ProcessUpdateAction(events->Front());
				}
				events->PopFront();
			}
			inputComponent->ExecuteAxis();
			if (InputControlledPawn)
			{
				InputControlledPawn->ExecuteAxis();
			}
			playerInput->ResetQueueEvents();
		}
	}
}