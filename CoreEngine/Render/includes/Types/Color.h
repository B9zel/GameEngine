#pragma once
#include <Math/includes/Vector.h>
#include <Math/includes/Vector4.h>

class LinearColor
{
public:

	LinearColor(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) : R(r), G(g), B(b), A(a)
	{
	}

	LinearColor(const LinearColor& Other) = default;
	LinearColor(LinearColor&& Other) = default;
	LinearColor(const FVector& Vec);
	LinearColor(const FVector4& Vec);

	LinearColor& operator=(const LinearColor& Other) = default;
	LinearColor& operator=(LinearColor&& Other) = default;

	LinearColor& operator=(const FVector& Vec);
	LinearColor& operator=(const FVector4& Vec);

	LinearColor operator+(const LinearColor& Other);
	LinearColor& operator+=(const LinearColor& Other);
	LinearColor operator-(const LinearColor& Other);
	LinearColor& operator-=(const LinearColor& Other);
	LinearColor operator*(const LinearColor& Other);
	LinearColor& operator*=(const LinearColor& Other);
	LinearColor operator/(const LinearColor& Other);
	LinearColor& operator/=(const LinearColor& Other);

public:

	union
	{
		struct
		{
			float R;
			float G;
			float B;
			float A;
		};
		float Data[4];
	};
};
