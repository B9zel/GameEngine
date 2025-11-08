#pragma once
#include <Runtime/includes/Controller.h>
#include <Runtime/includes/PlayerInput.h>
#include <Core/includes/Base.h>
#include <Runtime/includes/CameraManager.h>
#include <Runtime/includes/InputComponent.h>
#include <PlayerController.generated.h>


namespace CoreEngine
{
	namespace Runtime
	{
		class PlayerInput;
		class CameraManager;
		class InputComponent;
		class CameraComponent;

		RCLASS()
		class PlayerController : public Controller
		{
			GENERATED_BODY()

		public:

			PlayerController(const InitializeObject& Object);
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

			RPROPERTY();
			CameraManager* cameraManager;
			RPROPERTY();
			InputComponent* inputComponent;

			UniquePtr<PlayerInput> playerInput;

			UpdateActorFunction updateInput;

		private:

			bool canTransferEvent;
		};
	}
}