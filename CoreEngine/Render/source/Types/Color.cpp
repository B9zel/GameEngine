#include <Render/includes/Types/Color.h>

LinearColor::LinearColor(const FVector& Vec)
{
	R = Vec.GetX();
	G = Vec.GetY();
	B = Vec.GetZ();
	A = 1.0f;
}

LinearColor::LinearColor(const FVector4& Vec)
{
	R = Vec.GetX();
	G = Vec.GetY();
	B = Vec.GetZ();
	A = Vec.GetW();
}

LinearColor& LinearColor::operator=(const FVector& Vec)
{
	R = Vec.GetX();
	G = Vec.GetY();
	B = Vec.GetZ();
	A = 1.0f;

	return *this;
}

LinearColor& LinearColor::operator=(const FVector4& Vec)
{
	R = Vec.GetX();
	G = Vec.GetY();
	B = Vec.GetZ();
	A = Vec.GetW();

	return *this;
}

LinearColor LinearColor::operator+(const LinearColor& Other)
{
	return LinearColor(R + Other.R, G + Other.G, B + Other.B, A + Other.A);
}

LinearColor& LinearColor::operator+=(const LinearColor& Other)
{
	R += Other.R;
	G += Other.G;
	B += Other.B;
	A += Other.A;

	return *this;
}

LinearColor LinearColor::operator-(const LinearColor& Other)
{
	return LinearColor(R - Other.R, G - Other.G, B - Other.B, A - Other.A);
}

LinearColor& LinearColor::operator-=(const LinearColor& Other)
{
	R -= Other.R;
	G -= Other.G;
	B -= Other.B;
	A -= Other.A;

	return *this;
}

LinearColor LinearColor::operator*(const LinearColor& Other)
{
	return LinearColor(R * Other.R, G * Other.G, B * Other.B, A * Other.A);
}

LinearColor& LinearColor::operator*=(const LinearColor& Other)
{
	R *= Other.R;
	G *= Other.G;
	B *= Other.B;
	A *= Other.A;

	return *this;
}

LinearColor LinearColor::operator/(const LinearColor& Other)
{
	CORE_ASSERT((Other.R * Other.G * Other.B * Other.A) != 0.0f, "Cannot divide by zero");

	return LinearColor(R / Other.R, G / Other.G, B / Other.B, A / Other.A);
}

LinearColor& LinearColor::operator/=(const LinearColor& Other)
{
	CORE_ASSERT((Other.R * Other.G * Other.B * Other.A) != 0.0f, "Cannot divide by zero");

	R /= Other.R;
	G /= Other.G;
	B /= Other.B;
	A /= Other.A;

	return *this;
}

bool LinearColor::operator==(const LinearColor& Other) const
{
	return R == Other.R && G == Other.G && B == Other.B && A == Other.A;
}
bool LinearColor::operator!=(const LinearColor& Other) const
{
	return !(*this == Other);
}
