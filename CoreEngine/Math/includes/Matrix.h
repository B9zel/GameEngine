#pragma once

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
