#pragma once

#include <Math/includes/Vector.h>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>


using FMatrix4x4 = glm::mat4x4;
using DMatrix4x4 = glm::dmat4x4;

using FMatrix3x3 = glm::mat3x3;
using DMatrix3x3 = glm::dmat3x3;


template<class T>
typename T::value_type const* GetValuePtr(T const& value)
{
	return glm::value_ptr(value);
}


inline FMatrix4x4 Translate(const FMatrix4x4& matrix, const FVector& OffsetVec)
{
	return  glm::translate(matrix, OffsetVec.vector);
}

inline DMatrix4x4 Translate(const DMatrix4x4& matrix, const DVector& OffsetVec)
{
	return glm::translate(matrix, OffsetVec.vector);
}


inline FMatrix4x4 Rotate(const FMatrix4x4& matrix,const float Angle, const FVector& AxisVec)
{
	return glm::rotate(matrix, Angle, AxisVec.vector);
}

inline DMatrix4x4 Rotate(const DMatrix4x4& matrix, const double Angle, const DVector& AxisVec)
{
	return glm::rotate(matrix, Angle, AxisVec.vector);
}

inline FMatrix4x4 ToScale(const FMatrix4x4& matrix, const FVector& newScale)
{
	return  glm::scale(matrix, newScale.vector);
}

inline DMatrix4x4 ToScale(const DMatrix4x4& matrix, const DVector& newScale)
{
	return glm::scale(matrix, newScale.vector);
}