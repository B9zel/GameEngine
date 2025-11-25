#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#include <Editor/includes/EditorViewportClient.h>
#include <Core/includes/Engine.h>
#include <Core/includes/Application.h>
#include <Render/includes/Render.h>
#include <Core/includes/InputDevice.h>


namespace Editor
{
	EditorViewportClient::EditorViewportClient()
	{
		m_FieldOfView = 45;
		m_zFar = 1000;
		m_zNear = 0.1;

		m_leftOrtho = 50;
		m_rightOrtho = 50;
		m_bottomOrtho = 50;
		m_topOrtho = 50;

		m_ViewType = ETypeView::PERSPECTIVE;
		SpeedMove = 20;
		SpeedRotation = 80;
		SetRotation(FVector(0, -90, 0));
	}

	FMatrix4x4 EditorViewportClient::CreateProjection()
	{
		FVector2 ScreenSize = CoreEngine::Engine::Get()->GetScreenSize();
		FMatrix4x4 Projection;

		if (GetTypeProjection() == ETypeView::PERSPECTIVE)
		{
			Projection = CreatePerspectiveMatrix(Math::ToRadian(m_FieldOfView), static_cast<uint32>(ScreenSize.x), static_cast<uint32>(ScreenSize.y), m_zNear, m_zFar);
		}
		else
		{
			Projection = CreateOrthoMatrix(m_leftOrtho, m_rightOrtho, m_bottomOrtho, m_topOrtho, m_zNear, m_zFar);
		}
		return Projection;
	}

	void EditorViewportClient::SetRotation(const FVector& Rotate)
	{
		m_Transform.LookAt.SetX(cos(Math::ToRadian(Rotate.GetX())) * cos(Math::ToRadian(Rotate.GetY())));
		m_Transform.LookAt.SetY(sin(Math::ToRadian(Rotate.GetX())));
		m_Transform.LookAt.SetZ(cos(Math::ToRadian(Rotate.GetX())) * sin(Math::ToRadian(Rotate.GetY())));

		m_Transform.LookAt.SafeNormalize();

		m_Transform.Rotation = Rotate;
	}

	FMatrix4x4 EditorViewportClient::GetViewMatrix()
	{
		m_ViewMatrix = CreateMatrixLookAt(GetLocation(), GetLocation() + m_Transform.LookAt, FVector(0, 1, 0));
		return m_ViewMatrix;
	}

	void EditorViewportClient::Update(float DeltaTime, const bool IsHoveredViewport)
	{
		CoreEngine::Engine::Get()->GetRender()->SetViewProjectionMatrix(GetViewMatrix(), CreateProjection());
		//EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "{0}, {1}, {2}", m_Transform.LookAt.GetX(), m_Transform.LookAt.GetY(), m_Transform.LookAt.GetZ());
		EG_LOG(CoreEngine::CORE, ELevelLog::INFO, IsHoveredViewport);
			//EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "{0} {1} {2}", GetRotation().GetX(), GetRotation().GetY(), GetRotation().GetZ());
		if (!CoreEngine::InputDevice::GetIsButtonPressed(GLFW_MOUSE_BUTTON_RIGHT) || !IsHoveredViewport)
		{
			LastPosMouse = CoreEngine::InputDevice::GetMousePos();
			//EG_LOG(CoreEngine::CORE, ELevelLog::INFO, "{0}, {1}", LastPosMouse.x, LastPosMouse.y);
			return;
		}
		
		FVector NewPos = GetLocation();
		FVector NewRot = GetRotation();

		if (CoreEngine::InputDevice::GetIsKeyPressed(GLFW_KEY_W))
		{
			NewPos += m_Transform.LookAt * SpeedMove * DeltaTime;
		}
		if (CoreEngine::InputDevice::GetIsKeyPressed(GLFW_KEY_S))
		{
			NewPos -= m_Transform.LookAt * SpeedMove * DeltaTime;
		}
		if (CoreEngine::InputDevice::GetIsKeyPressed(GLFW_KEY_D))
		{
			NewPos += m_Transform.LookAt.Cross(FVector::UpVector) * SpeedMove * DeltaTime;
		}
		if (CoreEngine::InputDevice::GetIsKeyPressed(GLFW_KEY_A))
		{
			NewPos -= m_Transform.LookAt.Cross(FVector::UpVector) * SpeedMove * DeltaTime;
		}
		if (CoreEngine::InputDevice::GetIsKeyPressed(GLFW_KEY_Q))
		{
			NewPos -= FVector::UpVector * SpeedMove * DeltaTime;
		}
		if (CoreEngine::InputDevice::GetIsKeyPressed(GLFW_KEY_E))
		{
			NewPos += FVector::UpVector * SpeedMove * DeltaTime;
		}

		DVector2 Difference = CoreEngine::InputDevice::GetMousePos() - LastPosMouse;
		LastPosMouse = CoreEngine::InputDevice::GetMousePos();

		const float RealSpeed = SpeedRotation * DeltaTime;

		NewRot.SetY(NewRot.GetY() + Difference.x * RealSpeed);
		NewRot.SetX(Math::Clamp(NewRot.GetX() - Difference.y * RealSpeed, -90, 90));
	
			
		SetLocation(NewPos);
		SetRotation(NewRot);
	}

	const FVector& EditorViewportClient::GetLocation() const
	{
		return m_Transform.Location;
	}
	const FVector& EditorViewportClient::GetRotation() const
	{
		return m_Transform.Rotation;
	}

	ETypeView EditorViewportClient::GetTypeProjection() const
	{
		return m_ViewType;
	}

	void EditorViewportClient::SetLocation(const FVector& NewLocation)
	{
		m_Transform.Location = NewLocation;
	}

}