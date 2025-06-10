#pragma once
#include <glm/vec4.hpp>

template<class T>
class TVector3;

template<class T>
class TVector4
{
public:

	using TypeVector = glm::vec<4, T, glm::defaultp>;

public:

	TVector4(const T value = 0)
	{
		vector.x = value;
		vector.y = value;
		vector.z = value;
		vector.w = value;
	}

	TVector4(const TypeVector& Vec)
	{
		vector.x = Vec.x;
		vector.y = Vec.y;
		vector.z = Vec.z;
		vector.w = Vec.w;
	}

	TVector4(const TVector3<T>& Vec, const T ValueW = T())
	{
		vector.x = Vec.vector.x;
		vector.y = Vec.vector.y;
		vector.z = Vec.vector.z;
		vector.w = ValueW;
	}

	/*TVector4(TVector3<T>&& Vec, const T ValueW)
	{
		vector.x = Vec.vector.x;
		vector.y = Vec.vector.y;
		vector.z = Vec.vector.z;
		vector.w = ValueW;

		Vec.vector.x = 0;
		Vec.vector.y = 0;
		Vec.vector.z = 0;
	}*/

	TVector4(const T x, const T y, const T z, const T w)
	{
		vector.x = x;
		vector.y = y;
		vector.z = z;
		vector.w = w;
	}

	TVector4(const TVector4& Value)
	{
		vector.x = Value.vector.x;
		vector.y = Value.vector.y;
		vector.z = Value.vector.z;
		vector.w = Value.vector.w;
	}

	TVector4(TVector4&& Value)
	{
		vector.x = Value.vector.x;
		vector.y = Value.vector.y;
		vector.z = Value.vector.z;
		vector.w = Value.vector.w;

		Value.vector.x = 0;
		Value.vector.y = 0;
		Value.vector.z = 0;
		Value.vector.w = 0;
	}

	TVector4& operator=(const TVector4& Value)
	{
		vector.x = Value.vector.x;
		vector.y = Value.vector.y;
		vector.z = Value.vector.z;
		vector.w = Value.vector.w;

		return *this;
	}

	TVector4& operator=(TVector4&& Value)
	{
		vector.x = Value.vector.x;
		vector.y = Value.vector.y;
		vector.z = Value.vector.z;
		vector.z = Value.vector.w;

		Value.vector.x = 0;
		Value.vector.y = 0;
		Value.vector.z = 0;
		Value.vector.w = 0;

		return *this;
	}

	bool operator==(const TVector4& Value)
	{
		return vertor == Value.vector;
	}

	bool operator!=(const TVector4& Value)
	{
		return vertor != Value.vector;
	}

	TVector4& operator+=(const TVector4& Value)
	{
		vector += Value.vector;
		return *this;
	}

	TVector4& operator-=(const TVector4& Value)
	{
		vector -= Value.vector;
		return *this;
	}

	TVector4& operator*=(const TVector4& Value)
	{
		vector *= Value.vector;
		return *this;
	}

	TVector4& operator/=(const TVector4& Value)
	{
		vector /= Value.vector;
		return *this;
	}

	TVector4& operator%=(const TVector4& Value)
	{
		vector %= Value.vector;
		return *this;
	}

	TVector4 operator+(const TVector4& Value) const
	{
		TVector4 res(vector.x, vector.y, vector.z, vector.w);

		res += Value;
		return res;
	}

	TVector4 operator-(const TVector4& Value) const
	{
		TVector4 res(vector.x, vector.y, vector.z, vector.w);

		res -= Value;
		return res;
	}

	TVector4 operator*(const TVector4& Value) const
	{
		TVector4 res(vector.x, vector.y, vector.z, vector.w);

		res *= Value;
		return res;
	}

	TVector4 operator/(const TVector4& Value) const
	{
		TVector4 res(vector.x, vector.y, vector.z, vector.w);

		res /= Value;
		return res;
	}

	//TVector4 operator%(const TVector4& Value) const
	//{
	//	TVector4 res(vector.x, vector.y, vector.z);

	//	res %= Value;
	//	return res;
	//}

	TVector4 operator+(const T& Value) const
	{
		TVector4 res(vector.x + Value, vector.y + Value, vector.z + Value, vector.w + Value);

		return res;
	}

	TVector4 operator-(const T& Value) const
	{
		TVector4 res(vector.x - Value, vector.y - Value, vector.z - Value, vector.w - Value);

		return res;
	}

	TVector4 operator*(const T& Value) const
	{
		TVector4 res(vector.x * Value, vector.y * Value, vector.z * Value, vector.w * Value);

		return res;
	}

	TVector4 operator/(const T& Value) const
	{
		if (Value == static_cast<T>(0.0))
		{
			return FVector4(0, 0, 0, 0);
		}
		TVector4 res(vector.x / Value, vector.y / Value, vector.z / Value, vector.w / Value);

		return res;
	}

	TVector4& operator-()
	{
		vector.x = -vector.x;
		vector.y = -vector.y;
		vector.z = -vector.z;
		vector.w = -vector.w;

		return *this;
	}

	TVector4& operator++()
	{
		++vector;
		return *this;
	}

	TVector4& operator++(int)
	{
		TVector4 res(*this);
		++*this;

		return res;
	}

	TVector4& operator--()
	{
		--vector;
		return *this;
	}

	TVector4& operator--(int)
	{
		TVector4 res(*this);
		--*this;

		return res;
	}

	T operator[](unsigned int pos) const
	{
		return vector[pos];
	}

public:

	TVector4& Normalize()
	{
		vector = glm::normalize(vector);

		return *this;
	}

	TVector4& SafeNormalize()
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

	T Dot(const TVector4& Vec) const
	{
		return glm::dot(vector, Vec.vector);
	}

	TVector4 Cross(const TVector4& Vec) const
	{
		glm::vec<4, T, glm::defaultp> CrossVec = glm::cross(vector, Vec.vector);
		return TVector4(CrossVec.x, CrossVec.y, CrossVec.z, CrossVec.w);
	}

	T Length() const
	{
		return sqrt(LengthSquared());
	}

	T LengthSquared() const
	{
		return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w;
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

	inline T GetW() const
	{
		return vector.w;
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

	void SetW(const T w)
	{
		vector.z = z;
	}

public:

	TypeVector vector;
};