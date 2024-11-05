#pragma once
#include <Math/includes/Vector.h>
#include <Math/includes/Matrix.h>



class Transform
{
public:

	Transform() = default;
	~Transform() = default;

	Transform(const FVector& Location, const FVector& Rotation, const FVector& Scale);
	Transform(const Transform& Other);
	Transform(Transform&& Other);

	Transform& operator=(const Transform& OtherTransform);
	Transform& operator=(Transform&& OtherTransform);

public:

	void SetLocation(const FVector& NewLocation);
	void SetRotation(const FVector& NewRotation);
	void SetScale(const FVector& NewScale);
	void SetLocationAndRotation(const FVector& NewLocation, const FVector& NewRotation);

	void SetTransform(const FVector& Location, const FVector& Rotation, const FVector& Scale);


	FVector GetLocation() const;
	FVector GetRotation() const;
	FVector GetScale() const;
	
	Transform GetTransform() const;


	FMatrix4x4 ToMatrix() const;

private:

	FVector Location;
	FVector Scale;
	FVector Rotation;
};