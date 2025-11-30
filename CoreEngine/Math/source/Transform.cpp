#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#include <Math/includes/Transform.h>
#include <Math/includes/Matrix.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


FTransform::FTransform(const FVector& Location, const FVector& Rotation, const FVector& Scale) : Location(Location), Scale(Scale), Rotation(Rotation)
{
}

FTransform::FTransform(const FTransform& Other)
{
	this->operator=(Other);
}

FTransform::FTransform(FTransform&& Other) noexcept
{
	this->operator=(std::move(Other));
}

FTransform& FTransform::operator=(const FTransform& OtherTransform)
{
	Location = OtherTransform.Location;
	Scale = OtherTransform.Scale;
	Rotation = OtherTransform.Rotation;

	return *this;
}

FTransform& FTransform::operator=(FTransform&& OtherTransform) noexcept
{
	Location = std::move(OtherTransform.Location);
	Scale = std::move(OtherTransform.Scale);
	Rotation = std::move(OtherTransform.Rotation);

	return *this;
}

bool FTransform::operator==(const FTransform& Other) const
{
	return Other.Location.vector == Location.vector && Other.Rotation.vector == Rotation.vector && Other.Scale.vector == Scale.vector;
}

bool FTransform::operator!=(const FTransform& Other) const
{
	return !(this->operator==(Other));
}

void FTransform::SetLocation(const FVector& NewLocation)
{
	Location = NewLocation;
}

void FTransform::SetRotation(const FVector& NewRotation)
{
	Rotation = NewRotation;
}

void FTransform::SetScale(const FVector& NewScale)
{
	Scale = NewScale;
}

void FTransform::SetLocationAndRotation(const FVector& NewLocation, const FVector& NewRotation)
{
	SetLocation(NewLocation);
	SetRotation(NewRotation);
}

void FTransform::SetTransform(const FVector& Location, const FVector& Rotation, const FVector& Scale)
{
	SetLocation(Location);
	SetRotation(Rotation);
	SetScale(Scale);
}

FVector FTransform::GetLocation() const
{
	return Location;
}

FVector FTransform::GetRotation() const
{
	return Rotation;
}

FVector FTransform::GetScale() const
{
	return Scale;
}

FVector& FTransform::GetLocationRef()
{
	return Location;
}

FVector& FTransform::GetRotationRef()
{
	return Rotation;
}

FVector& FTransform::GetScaleRef()
{
	return Scale;
}


FTransform FTransform::GetTransform() const
{
	return FTransform(*this);
}

FMatrix4x4 FTransform::ToMatrix() const
{
	//FMatrix4x4 RotatMat =  Math::Rotate(FMatrix4x4(1.0f), Rotat.GetX(), FVector(1.0f, 0.0f, 0.0f))
	//	* Math::Rotate(FMatrix4x4(1.0f), Rotat.GetY(), FVector(0.0f, 1.0f, 0.0f))
	//	* Math::Rotate(FMatrix4x4(1.0f), Rotat.GetZ(), FVector(0.0f, 0.0f, 1.0f));

	return Math::Translate(FMatrix4x4(1.0f), Location)  * Math::ToScale(FMatrix4x4(1.0f), Scale) * glm::toMat4(glm::quat(Math::ToRadianVector(Rotation).vector));
}

