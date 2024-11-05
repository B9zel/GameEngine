#include <Math/includes/Transform.h>
#include <Math/includes/Matrix.h>
#include <Math/includes/Math.h>



Transform::Transform(const FVector& Location, const FVector& Rotation, const FVector& Scale) : Location(Location), Scale(Scale), Rotation(Rotation)
{
}

Transform::Transform(const Transform& Other)
{
    this->operator=(Other);
}

Transform::Transform(Transform&& Other)
{
    this->operator=(std::move(Other));
}

Transform& Transform::operator=(const Transform& OtherTransform)
{
    Location = OtherTransform.Location;
    Scale = OtherTransform.Scale;
    Rotation = OtherTransform.Rotation;
   
    return *this;
}

Transform& Transform::operator=(Transform&& OtherTransform)
{
    Location = std::move(OtherTransform.Location);
    Scale = std::move(OtherTransform.Scale);
    Rotation = std::move(OtherTransform.Rotation);

    return *this;
}

void Transform::SetLocation(const FVector& NewLocation)
{
    Location = NewLocation;
}

void Transform::SetRotation(const FVector& NewRotation)
{
    Rotation = NewRotation;
}

void Transform::SetScale(const FVector& NewScale)
{
    Scale = NewScale;
}

void Transform::SetLocationAndRotation(const FVector& NewLocation, const FVector& NewRotation)
{
    SetLocation(NewLocation);
    SetRotation(NewRotation);
}

void Transform::SetTransform(const FVector& Location, const FVector& Rotation, const FVector& Scale)
{
    SetLocation(Location);
    SetRotation(Rotation);
    SetScale(Scale);
}

FVector Transform::GetLocation() const
{
    return Location;
}

FVector Transform::GetRotation() const
{
    return Rotation;
}

FVector Transform::GetScale() const
{
    return Scale;
}


Transform Transform::GetTransform() const
{
    return Transform(*this);
}

FMatrix4x4 Transform::ToMatrix() const
{
    FMatrix4x4 matrix(1);
    matrix = Translate(matrix, Location);
    matrix = ToScale(matrix, Scale);

    matrix = Rotate(matrix, Rotation.GetX(), FVector(1.0f, 0.0f, 0.0f));
    matrix = Rotate(matrix, Rotation.GetY(), FVector(0.0f, 1.0f, 0.0f));
    matrix = Rotate(matrix, Rotation.GetZ(), FVector(0.0f, 0.0f, 1.0f));

    return matrix;
}
