#pragma once
#include <Math/includes/Vector.h>
#include <Math/includes/Matrix.h>
#include <Runtime/includes/Enums/ViewUtil.h>
#include <Core/includes/Dispatcher.h>

namespace Editor
{
	struct ViewportTransform
	{
		FVector Location;
		FVector Rotation;

		FVector LookAt;
	};



	class EditorViewportClient
	{
	public:

		EditorViewportClient();

	public:

		void SetLocation(const FVector& NewLocation);
		void SetRotation(const FVector& NewRotate);

		const FVector& GetLocation() const;
		const FVector& GetRotation() const;
		FMatrix4x4 CreateProjection(const uint32 WidhtScreen, const uint32 HeightScreen);
		ETypeView GetTypeProjection() const;
		FMatrix4x4 GetViewMatrix();

		void SetViewportSize(const uint32 Width, const uint32 Height);
		uint32 GetViewportWidth() const;
		uint32 GetViewportHeight() const;

		void Update(float DeltaTime, const bool IsHoveredViewport);

	public:

		Dispatcher<bool> EventActiveMove;

	private:


		ViewportTransform m_Transform;
		FMatrix4x4 m_ViewMatrix;

		ETypeView m_ViewType;

		float SpeedMove;
		float SpeedRotation;
		DVector2 LastPosMouse;

		float m_FieldOfView;
		float m_zNear;
		float m_zFar;

		float m_leftOrtho;
		float m_rightOrtho;
		float m_bottomOrtho;
		float m_topOrtho;

		uint32 m_ViewportWidth{0};
		uint32 m_ViewportHeight{0};
	};
}