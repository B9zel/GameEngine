#pragma once
#include <Math/includes/Vector.h>
#include <Math/includes/Matrix.h>



class FTransform
{
public:

	FTransform() = default;
	~FTransform() = default;

	FTransform(const FVector& Location, const FVector& Rotation, const FVector& Scale);
	FTransform(const FTransform& Other);
	FTransform(FTransform&& Other) noexcept;

	FTransform& operator=(const FTransform& OtherTransform);
	FTransform& operator=(FTransform&& OtherTransform) noexcept;

	bool operator==(const FTransform& Other) const;
	bool operator!=(const FTransform& Other) const;

public:

	void SetLocation(const FVector& NewLocation);
	void SetRotation(const FVector& NewRotation);
	void SetScale(const FVector& NewScale);
	void SetLocationAndRotation(const FVector& NewLocation, const FVector& NewRotation);

	void SetTransform(const FVector& Location, const FVector& Rotation, const FVector& Scale);


	FVector GetLocation() const;
	FVector GetRotation() const;
	FVector GetScale() const;
	FVector& GetLocationRef();
	FVector& GetRotationRef();
	FVector& GetScaleRef();
	
	FTransform GetTransform() const;


	FMatrix4x4 ToMatrix() const;
	
private:

	FVector Location;
	FVector Scale;
	FVector Rotation;
};