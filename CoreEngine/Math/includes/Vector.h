#pragma once

#include <Math/includes/Vector4.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/geometric.hpp>
//#include <type_traits>
#include <Core/includes/Base.h>
//#include <spdlog/fmt/ostr.h>

//#include <cstdio>
//#include <format>


template<class T>
class TVector3;

using FVector4 = TVector4<float>;
using DVector4 = TVector4<double>;
using IVector4 = TVector4<int>;
using UVector4 = TVector4<unsigned int>;

using FVector = TVector3<float>;
using DVector = TVector3<double>;
using IVector = TVector3<int>;
using UVector = TVector3<unsigned int>;

using FVector2 = glm::vec2;
using DVector2 = glm::dvec2;
using IVector2 = glm::ivec2;
using UVector2 = glm::uvec2;





template<class T>
class TVector3
{
public:

	static const TVector3<T> ForwardVector;
	static const TVector3<T> BackVector;
	static const TVector3<T> UpVector;
	static const TVector3<T> DownVector;
	static const TVector3<T> RightVector;
	static const TVector3<T> LeftVector;

public:

	TVector3(const glm::vec3& Vector)
	{
		vector = Vector;
	}

	TVector3(const T value = 0)
	{
		vector.x = value;
		vector.y = value;
		vector.z = value;
	}

	TVector3(const T x, const T y, const T z)
	{
		vector.x = x;
		vector.y = y;
		vector.z = z;
	}

	TVector3(const TVector3& OtherVec)
	{
		vector.x = OtherVec.vector.x;
		vector.y = OtherVec.vector.y;
		vector.z = OtherVec.vector.z;
	}

	TVector3(TVector3&& OtherVec)
	{
		vector.x = OtherVec.vector.x;
		vector.y = OtherVec.vector.y;
		vector.z = OtherVec.vector.z;

		OtherVec.vector.x = 0;
		OtherVec.vector.y = 0;
		OtherVec.vector.z = 0;
	}

	TVector3& operator=(const TVector3& otherVec)
	{
		vector.x = otherVec.vector.x;
		vector.y = otherVec.vector.y;
		vector.z = otherVec.vector.z;

		return *this;
	}

	TVector3& operator=(TVector3&& otherVec)
	{
		vector.x = otherVec.vector.x;
		vector.y = otherVec.vector.y;
		vector.z = otherVec.vector.z;

		otherVec.vector.x = 0;
		otherVec.vector.y = 0;
		otherVec.vector.z = 0;

		return *this;
	}

	bool operator==(const TVector3& otherVec)
	{
		return vertor == otherVec.vector;
	}

	bool operator!=(const TVector3& otherVec)
	{
		return vertor != otherVec.vector;
	}

	TVector3& operator+=(const TVector3& otherVec)
	{
		vector += otherVec.vector;
		return *this;
	}

	TVector3& operator-=(const TVector3& otherVec)
	{
		vector -= otherVec.vector;
		return *this;
	}

	TVector3& operator*=(const TVector3& otherVec)
	{
		vector *= otherVec.vector;
		return *this;
	}

	TVector3& operator/=(const TVector3& otherVec)
	{
		vector /= otherVec.vector;
		return *this;
	}

	TVector3& operator%=(const TVector3& otherVec)
	{
		vector %= otherVec.vector;
		return *this;
	}

	TVector3 operator+(const TVector3& otherVec) const
	{
		TVector3 res(vector.x, vector.y, vector.z);

		res += otherVec;
		return res;
	}

	TVector3 operator-(const TVector3& otherVec) const
	{
		TVector3 res(vector.x, vector.y, vector.z);

		res -= otherVec;
		return res;
	}

	TVector3 operator*(const TVector3& otherVec) const
	{
		TVector3 res(vector.x, vector.y, vector.z);

		res *= otherVec;
		return res;
	}

	TVector3 operator/(const TVector3& otherVec) const
	{
		TVector3 res(vector.x, vector.y, vector.z);

		res /= otherVec;
		return res;
	}

	//TVector3 operator%(const TVector3& otherVec) const
	//{
	//	TVector3 res(vector.x, vector.y, vector.z);

	//	res %= otherVec;
	//	return res;
	//}

	TVector3 operator+(const T& otherVec) const
	{
		TVector3 res(vector.x + otherVec, vector.y + otherVec, vector.z + otherVec);

		return res;
	}

	TVector3 operator-(const T& otherVec) const
	{
		TVector3 res(vector.x - otherVec, vector.y - otherVec, vector.z - otherVec);

		return res;
	}

	TVector3 operator*(const T& otherVec) const
	{
		TVector3 res(vector.x * otherVec, vector.y * otherVec, vector.z * otherVec);

		return res;
	}

	TVector3 operator/(const T& otherVec) const
	{
		if (otherVec == static_cast<T>(0.0))
		{
			return FVector(0, 0, 0);
		}
		TVector3 res(vector.x / otherVec, vector.y / otherVec, vector.z / otherVec);

		return res;
	}

	TVector3& operator-()
	{
		vector.x = -vector.x;
		vector.y = -vector.y;
		vector.z = -vector.z;

		return *this;
	}

	TVector3& operator++()
	{
		++vector;
		return *this;
	}

	TVector3& operator++(int)
	{
		TVector3 res(*this);
		++*this;

		return res;
	}

	TVector3& operator--()
	{
		--vector;
		return *this;
	}

	TVector3& operator--(int)
	{
		TVector3 res(*this);
		--*this;

		return res;
	}

	const T& operator[](unsigned int pos) const
	{
		return vector[pos];
	}
	T& operator[](unsigned int pos)
	{
		return vector[pos];
	}

public:

	TVector3& Normalize()
	{
		vector = glm::normalize(vector);

		return *this;
	}

	TVector3& SafeNormalize()
	{
		T Squarelength = LengthSquared();
		if (Squarelength == static_cast<T>(0))
		{
			return *this;
		}

		T inv_Length = glm::inversesqrt(Squarelength);
		vector.x *= inv_Length;
		vector.y *= inv_Length;
		vector.z *= inv_Length;

		return *this;
	}

	T Dot(const TVector3& Vec) const
	{
		return glm::dot(vector, Vec.vector);
	}

	TVector3 Cross(const TVector3& Vec) const
	{
		glm::vec<3, T, glm::defaultp> CrossVec = glm::cross(vector, Vec.vector);
		return TVector3(CrossVec.x, CrossVec.y, CrossVec.z);
	}

	T Length() const
	{
		return sqrt(LengthSquared());
	}

	T LengthSquared() const
	{
		return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
	}

	inline T GetX() const
	{
		return vector.x;
	}

	inline T GetY() const
	{
		return vector.y;
	}

	inline T GetZ() const
	{
		return vector.z;
	}

	void SetX(const T x)
	{
		vector.x = x;
	}

	void SetY(const T y)
	{
		vector.y = y;
	}

	void SetZ(const T z)
	{
		vector.z = z;
	}

	//String ConvertToString() const;


public:

	glm::vec<3, T, glm::defaultp> vector;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const TVector3<T>& Vec)
{
	os << "X: ";
	os << Vec.GetX();
	os << " Y: ";
	os << Vec.GetY();
	os << " Z: ";
	os << Vec.GetZ();

	return os;
}
//template<class T>
//String TVector3<T>::ConvertToString() const
//{
//	fmt
//		return std::format("X: {} Y: {} Z: {}", static_cast<T>(vector.x), static_cast<T>(vector.y), static_cast<T>(vector.z));
//}


const FVector FVector::ForwardVector = FVector(0.0f, 0.0f, -1.0f);
const FVector FVector::BackVector = FVector(0.0f, 0.0f, 1.0f);
const FVector FVector::UpVector = FVector(0.0f, 1.0f, 0.0f);
const FVector FVector::DownVector = FVector(0.0f, -1.0f, 0.0f);
const FVector FVector::RightVector = FVector(1.0f, 0.0f, 0.0f);
const FVector FVector::LeftVector = FVector(-1.0f, 0.0f, 0.0f);
const DVector DVector::ForwardVector = DVector(0.0, 0.0, -1.0);
const DVector DVector::BackVector = DVector(0.0, 0.0, 1.0);
const DVector DVector::UpVector = DVector(0.0, 1.0, 0.0);
const DVector DVector::DownVector = DVector(0.0, -1.0, 0.0);
const DVector DVector::RightVector = DVector(1.0, 0.0, 0.0);
const DVector DVector::LeftVector = DVector(-1.0, 0.0, 0.0);