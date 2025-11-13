#pragma once

#include <Math/includes/Vector.h>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Core/includes/Base.h>
#include <Math/includes/Math.h>


using FMatrix4x4 = glm::mat4x4;
using DMatrix4x4 = glm::dmat4x4;

using FMatrix3x3 = glm::mat3x3;
using DMatrix3x3 = glm::dmat3x3;


template<class T>
typename T::value_type const* GetValuePtr(T const& value)
{
	return glm::value_ptr(value);
}

inline FMatrix4x4 CreateMatrixLookAt(const FVector& Position, const FVector& TargetPosition, const FVector& UpDirection)
{
	return glm::lookAt(Position.vector, TargetPosition.LengthSquared() != 0.0f ? TargetPosition.vector : FVector::ForwardVector.vector, UpDirection.vector);
}

inline FMatrix4x4 CreatePerspectiveMatrix(const float fieldOfView, uint32 Width, uint32 Height, const float zNear, const float zFar)
{
	return glm::perspective(fieldOfView, static_cast<float>(Width) / static_cast<float>(Height == 0.0 ? 1.0 : Height), zNear, zFar);
}

inline FMatrix4x4 CreateOrthoMatrix(const float Left, const float Right, const float Bottom, const float Top, const float zNear, const float zFar)
{
	return glm::ortho(Left, Right, Bottom, Top, zNear, zFar);
}


inline FMatrix4x4 Translate(const FMatrix4x4& matrix, const FVector& OffsetVec)
{
	return glm::translate(matrix, OffsetVec.vector);
}

inline DMatrix4x4 Translate(const DMatrix4x4& matrix, const DVector& OffsetVec)
{
	return glm::translate(matrix, OffsetVec.vector);
}


inline FMatrix4x4 Rotate(const FMatrix4x4& matrix, const float Angle, const FVector& AxisVec)
{
	return glm::rotate(matrix, Math::ToRadian(Angle), AxisVec.vector);
}

inline DMatrix4x4 Rotate(const DMatrix4x4& matrix, const double Angle, const DVector& AxisVec)
{
	return glm::rotate(matrix, Math::ToRadian(Angle), AxisVec.vector);
}

inline FMatrix4x4 ToScale(const FMatrix4x4& matrix, const FVector& newScale)
{
	return  glm::scale(matrix, newScale.vector);
}

inline DMatrix4x4 ToScale(const DMatrix4x4& matrix, const DVector& newScale)
{
	return glm::scale(matrix, newScale.vector);
}