#pragma once
#include <Runtime/includes/Controller.h>
#include <Runtime/includes/PlayerInput.h>
#include <Core/includes/Base.h>


namespace CoreEngine
{
	namespace Runtime
	{
		class PlayerInput;
		class CameraManager;
		class InputComponent;
		class CameraComponent;

		class PlayerController : public Controller
		{
		public:

			PlayerController();
			~PlayerController() = default;

		public:

			virtual void Update(float deltaTime) override;

		protected:

			virtual void OnPossess(Pawn* NewPawn) override;
			virtual void OnRegistered() override;
			virtual void SetupInputComponent();

		private:

			void OnTakeEvent(Event& event);
			void OnTakeProcessedEvent(Event& event);

			void ProcessInput(float deltaTime);

		protected:

			ObjectPtr<CameraManager> cameraManager;
			ObjectPtr<InputComponent> inputComponent;
			UniquePtr<PlayerInput> playerInput;

			UpdateActorFunction updateInput;

		private:

			bool canTransferEvent;
		};
	}
}