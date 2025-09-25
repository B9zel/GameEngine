#pragma once
#include <Runtime/includes/Actor.h>
#include <Runtime/includes/CameraComponent.h>
#include <CameraManager.generated.h>

namespace CoreEngine
{
	namespace Runtime
	{
		class CameraComponent;
		class PlayerController;

		RCLASS()
			class CameraManager : public Actor
		{
			GENERATED_BODY()

		public:

			CameraManager();

		public:

			virtual void Update(float deltaTime) override;
			virtual void BeginPlay() override;
			virtual void CameraUpdate(float deltaTime);

			void SetOwningPlayerController(PlayerController* NewController);

			FMatrix4x4 GetViewMatrix();
			FMatrix4x4 CreateProjection();
			FVector GetLocationCamera() const;

			ETypeView GetTypeProjection() const;

			FVector GetActiveCameraDirection() const;

		private:

			float m_FieldOfView;
			float m_zNear;
			float m_zFar;

			float m_leftOrtho;
			float m_rightOrtho;
			float m_bottomOrtho;
			float m_topOrtho;

			PlayerController* OwnController;
			CameraComponent* m_ActiveCamera;
		};
	}
}