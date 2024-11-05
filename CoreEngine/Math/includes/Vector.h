#pragma once

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <type_traits>


template<class T>
class TVector3;

using FVector4 = glm::vec4;
using DVector4 = glm::dvec4;
using IVector4 = glm::ivec4;
using UVector4 = glm::uvec4;

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

	TVector3(T x = 0, T y = 0, T z = 0)
	{
		vector.x = x;
		vector.y = y;
		vector.z = z;
	}

	TVector3(const TVector3& OtherVec)
	{
		this->operator=(OtherVec);
	}

	TVector3(TVector3&& OtherVec)
	{
		this->operator=(std::move(OtherVec));
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

	TVector3 operator+(const TVector3& otherVec)
	{
		TVector3 res(vector.x, vector.y, vector.z);

		res += otherVec;
		return res;
	}

	TVector3 operator-(const TVector3& otherVec)
	{
		TVector3 res(vector.x, vector.y, vector.z);

		res -= otherVec;
		return res;
	}

	TVector3 operator*(const TVector3& otherVec)
	{
		TVector3 res(vector.x, vector.y, vector.z);

		res *= otherVec;
		return res;
	}

	TVector3 operator/(const TVector3& otherVec)
	{
		TVector3 res(vector.x, vector.y, vector.z);

		res /= otherVec;
		return res;
	}

	TVector3 operator%(const TVector3& otherVec)
	{
		TVector3 res(vector.x, vector.y, vector.z);

		res %= otherVec;
		return res;
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

	T operator[](unsigned int pos)
	{
		return vector[pos];
	}

public:

	TVector3& Normalize()
	{
		vector = glm::normalize(vector);

		return *this;
	}

	T Dot(const TVector3& Vec)
	{
		return glm::dot(vector, Vec.vector);
	}

	TVector3 Cross(const TVector3& Vec)
	{
		return glm::cross(Vector, Vec);
	}

	T Length() const
	{
		return sqrt(LengthSquared());
	}

	T LengthSquared() const
	{
		return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
	}

	T GetX() const
	{
		return vector.x;
	}


	T GetY() const
	{
		return vector.y;
	}


	T GetZ() const
	{
		return vector.z;
	}

public:
	
	glm::vec<3, T, glm::defaultp> vector;
};
