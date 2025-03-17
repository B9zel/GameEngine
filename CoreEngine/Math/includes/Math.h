#pragma once
#include <glm/glm.hpp>
#include <Math/includes/Vector.h>


struct Math
{
	template<class T1, class T2 = T1, class T3 = T1 >
	static inline T1 Clamp(T1 current, T2 min, T3 max)
	{
		return current < min ? min : current > max ? max : current;
	}

	template<class T1, class T2 = T1 >
	static inline T1& Select(bool isA, const T1& A, const T2& B)
	{
		return isA ? A : B;
	}

	static inline float ToRadian(float degrees)
	{
		return glm::radians(degrees);
	}

	static inline double ToRadian(double degrees)
	{
		return glm::radians(degrees);
	}

	static inline FVector ToRadianVector(FVector degrees)
	{
		return FVector(ToRadian(degrees.GetX()), ToRadian(degrees.GetY()), ToRadian(degrees.GetZ()));
	}

	static inline float ToDegrees(float radian)
	{
		return glm::degrees(radian);
	}

	static inline double ToDegrees(double radian)
	{
		return glm::degrees(radian);
	}
};
