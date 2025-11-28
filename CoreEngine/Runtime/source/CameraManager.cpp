#include <Runtime/includes/CameraManager.h>
#include <Math/includes/Matrix.h>
#include <Runtime/includes/PlayerController.h>
#include <Runtime/includes/Pawn.h>
#include <Runtime/includes/Actor.h>
#include <Render/includes/Render.h>



namespace CoreEngine
{
	namespace Runtime
	{
		CameraManager::CameraManager(const InitializeObject& Object) : Actor(Object)
		{ 
			m_FieldOfView = 45;
			m_zFar = 1000;
			m_zNear = 0.1;

			m_leftOrtho = 50;
			m_rightOrtho = 50;
			m_bottomOrtho = 50;
			m_topOrtho = 50;

			m_ActiveCamera = nullptr;
			OwnController = nullptr;
		}
		void CameraManager::Update(float deltaTime)
		{
			Actor::Update(deltaTime);

			CameraUpdate(deltaTime);
		}

		void CameraManager::BeginPlay()
		{
			if (auto* ControlledPawn = OwnController->GetControlledPawn())
			{
				for (auto& Camera : ControlledPawn->FindComponentsByClass<CameraComponent>())
				{
					if (Camera->GetIsActive())
					{
						m_ActiveCamera = Camera;
					}
				}
			}
		}

		void CameraManager::CameraUpdate(float deltaTime)
		{
			FMatrix4x4 View = GetViewMatrix();

			Engine::Get()->GetRender()->SetViewProjectionMatrix(View, CreateProjection());
		}

		void CameraManager::SetOwningPlayerController(PlayerController* NewController)
		{
			OwnController = NewController;
		}

		FMatrix4x4 CameraManager::GetViewMatrix()
		{
			if (m_ActiveCamera && m_ActiveCamera->GetIsActive()) return m_ActiveCamera->GetViewMatrix();

			Pawn* OwnerPawn = OwnController->GetControlledPawn();

			if (OwnerPawn)
			{
				for (auto& Camera : OwnerPawn->FindComponentsByClass<CameraComponent>())
				{
					if (Camera->GetIsActive())
					{
						m_ActiveCamera = Camera;
						return Camera->GetViewMatrix();
					}
				}
			}
			if (OwnController)
			{
				return Math::CreateMatrixLookAt(OwnController->GetActorLocation(), OwnController->GetActorLocation() + OwnController->GetActorForwardVector(), FVector(0, 1, 0));
			}

			return FMatrix4x4(1);
		}

		FMatrix4x4 CameraManager::CreateProjection()
		{
			FVector2 ScreenSize = Engine::Get()->GetScreenSize();
			FMatrix4x4 Projection;

			if (GetTypeProjection() == ETypeView::PERSPECTIVE)
			{
				Projection = Math::CreatePerspectiveMatrix(Math::ToRadian(m_FieldOfView), static_cast<uint32>(ScreenSize.x), static_cast<uint32>(ScreenSize.y), m_zNear, m_zFar);
			}
			else
			{
				Projection = Math::CreateOrthoMatrix(m_leftOrtho, m_rightOrtho, m_bottomOrtho, m_topOrtho, m_zNear, m_zFar);
			}
			return Projection;
		}

		ETypeView CameraManager::GetTypeProjection() const
		{
			Pawn* Owner = OwnController->GetControlledPawn();

			if (Owner == nullptr)
			{
				return ETypeView::PERSPECTIVE;
			}

			if (auto* Camera = Owner->FindComponentByClass<CameraComponent>())
			{
				return Camera->GetTypeView();
			}

			return ETypeView::PERSPECTIVE;
		}

		FVector CameraManager::GetLocationCamera() const
		{
			//Pawn* Owner = OwnController->GetControlledPawn();
			if (m_ActiveCamera)
			{
				return m_ActiveCamera->GetComponentLocation();
			}
			/*if (Owner == nullptr)
			{
				return OwnController->GetActorLocation();
			}*/
			return Owner->GetActorLocation();
		}

		FVector CameraManager::GetActiveCameraDirection() const
		{
			return m_ActiveCamera != nullptr ? m_ActiveCamera->GetForwardVector() : FVector(0.0f, 0.0f, -1.0f);
		}
	}
}

