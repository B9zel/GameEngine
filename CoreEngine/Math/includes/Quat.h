#pragma once
#include <glm/gtc/quaternion.hpp>
#include <Math/includes/Vector.h>

using FQuat = glm::quat;
using DQuat = glm::dquat;

namespace Math
{
	FVector ToEuler(const FQuat& quat);
} // namespace Math
