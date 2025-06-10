#pragma once
#include <Math/includes/Vector.h>
#include <Runtime/includes/Enums/TypeLight.h>

namespace CoreEngine
{
	struct alignas(32) SimplyPointLightProxy
	{
		FVector Color;
		float Intencity;
		FVector Location;
		float Constant;
		float Linear;
		float Quadratic;
	};

	struct alignas(16) SimplyDirectionLightProxy
	{
		FVector Color;
		float Intencity;
		FVector Direction;
	};

	struct SimpleSpotLightProxy
	{
		FVector4 Color;
		FVector4 Direction;
		FVector Location;

		float Intencity;
		float CutOff;
		float OuterCutOff;
		float Constant;
		float Linear;
		float Quadratic;
	};

	class alignas(16) LightProxy
	{
	public:

		virtual const FVector& GetColor();
		virtual float GetIntencity() const;

		/*
		* @param NewColor input x, y, z from 0 to 1
		*/
		void SetColor(const FVector& NewColor);
		void SetIntencity(const float NewIntencity);
		virtual const ETypeLight GetTypeLight() const = 0;

	protected:

		FVector Color;
		float Intencity;
	};

	class alignas(16) DirectionLightProxy : public LightProxy
	{
	public:

		void SetDirection(const FVector& NewDirection);
		const FVector& GetDirection() const;

		virtual const ETypeLight GetTypeLight() const override;

	private:

		FVector m_Direction;
	};


	class alignas(16) PointLightProxy : public LightProxy
	{
	public:

		void SetLocation(const FVector& NewLocation);
		void SetConstant(const float NewConstant);
		void SetLinear(const float NewLinear);
		void SetQuadratic(const float NewQuadratic);

		const FVector& GetLocation() const;
		float GetConstant() const;
		float GetLinear() const;
		float GetQuadratic() const;

		virtual const ETypeLight GetTypeLight() const override;

	private:

		FVector m_Location;
		/*
		*  Use for formulas
		*							1
		* -----------------------------------------------------
		* Constant + Linear * Distance + Quadratic * Distance^2
		*/
		float m_Constant;
		float m_Linear;
		float m_Quadratic;
	};

	class alignas(16) SpotLightProxy : public LightProxy
	{
	public:

		void SetLocation(const FVector& NewLocation);
		void SetDirection(const FVector& NewDirection);
		void SetConstant(const float NewConstant);
		void SetLinear(const float NewLinear);
		void SetQuadratic(const float NewQuadratic);
		void SetCutOff(const float NewCutOff);
		void SetOuterCutOff(const float NewOuterCutOff);

		const FVector& GetLocation() const;
		const FVector& GetDirection() const;
		float GetConstant() const;
		float GetLinear() const;
		float GetQuadratic() const;
		float GetCutOff() const;
		float GetOuterCutOff() const;

		virtual const ETypeLight GetTypeLight() const override;

	private:

		FVector m_Location;
		FVector m_Direction;

		float m_CutOff;
		float m_OuterCutOff;
		/*
		*  Use for formulas
		*							1
		* -----------------------------------------------------
		* Constant + Linear * Distance + Quadratic * Distance^2
		*/
		float m_Constant;
		float m_Linear;
		float m_Quadratic;
	};

}