#include <Math/includes/Matrix.h>


namespace Math
{
	bool Math::DecomposeMatrix(const FMatrix4x4& matrix, FVector& OutLocation, FVector& OutRotation, FVector& OutScale)
	{
		using namespace glm;
		using T = float;

		FMatrix4x4 LocalMatrix(matrix);

		if (epsilonEqual(LocalMatrix[3][3], static_cast<T>(0), epsilon<T>()))
			return false;



		if (
			epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
		{

			// Clear the perspective partition
			LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
			LocalMatrix[3][3] = static_cast<T>(1);
		}



		OutLocation = FVector(LocalMatrix[3]);
		LocalMatrix[3] = FVector4(0, 0, 0, LocalMatrix[3].w).vector;

		FVector Row[3], Pdum3;
		// Now get scale and shear.
		for (length_t i = 0; i < 3; ++i)
			for (length_t j = 0; j < 3; ++j)
				Row[i][j] = LocalMatrix[i][j];
		// Compute X scale factor and normalize first row.
		OutScale.vector.x = length(Row[0].vector);// v3Length(Row[0]);

		Row[0] = detail::scale(Row[0].vector, static_cast<T>(1));
		// Now, compute Y scale and normalize 2nd row.
		OutScale.vector.y = length(Row[1].vector);
		Row[1] = detail::scale(Row[1].vector, static_cast<T>(1));

		// Next, get Z scale and normalize 3rd row.
		OutScale.vector.z = length(Row[2].vector);
		Row[2] = detail::scale(Row[2].vector, static_cast<T>(1));


		OutRotation.vector.y = asin(-Row[0][2]);
		if (cos(OutRotation.vector.y) != 0) {
			OutRotation.vector.x = atan2(Row[1][2], Row[2][2]);
			OutRotation.vector.z = atan2(Row[0][1], Row[0][0]);
		}
		else {
			OutRotation.vector.x = atan2(-Row[2][0], Row[1][1]);
			OutRotation.vector.z = 0;
		}

		return true;
		
	}
}