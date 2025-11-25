#pragma once
#include <Math/includes/Vector.h>
#include <Math/includes/Matrix.h>
#include <Runtime/includes/Enums/ViewUtil.h>

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
		FMatrix4x4 CreateProjection();
		ETypeView GetTypeProjection() const;
		FMatrix4x4 GetViewMatrix();

		void Update(float DeltaTime, const bool IsHoveredViewport);

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
	};
}