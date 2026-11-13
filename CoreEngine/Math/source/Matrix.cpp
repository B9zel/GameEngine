#include <Math/includes/Matrix.h>
#include <Math/includes/Quat.h>

namespace Math
{
	FMatrix4x4 SetMatrixRotation(const FMatrix4x4& matrix, const FVector& newRotation)
	{
		glm::vec3 Position = glm::vec3(matrix[3]);

		glm::vec3 Scale(glm::length(glm::vec3(matrix[0])), glm::length(glm::vec3(matrix[1])), glm::length(glm::vec3(matrix[2])));

		glm::mat4 Rotation = glm::mat4_cast(FQuat(Math::ToRadianVector(newRotation.vector).vector)); // glm::quat

		return glm::translate(glm::mat4(1.0f), Position) * Rotation * glm::scale(glm::mat4(1.0f), Scale);
	}

	bool DecomposeLocationMatrix(const FMatrix4x4& matrix, FVector& Location)
	{
		using namespace glm;
		using T = float;

		FMatrix4x4 LocalMatrix(matrix);

		if (epsilonEqual(LocalMatrix[3][3], static_cast<T>(0), epsilon<T>())) return false;

		if (epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) || epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
		{

			// Clear the perspective partition
			LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
			LocalMatrix[3][3] = static_cast<T>(1);
		}

		Location = FVector(LocalMatrix[3]);
		return true;
	}
	bool Math::DecomposeMatrix(const FMatrix4x4& matrix, FVector& OutLocation, FVector& OutRotation, FVector& OutScale)
	{
		using namespace glm;
		using T = float;

		FMatrix4x4 LocalMatrix(matrix);

		if (epsilonEqual(LocalMatrix[3][3], static_cast<T>(0), epsilon<T>())) return false;

		if (epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) || epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
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
		OutScale.vector.x = length(Row[0].vector); // v3Length(Row[0]);

		Row[0] = detail::scale(Row[0].vector, static_cast<T>(1));
		// Now, compute Y scale and normalize 2nd row.
		OutScale.vector.y = length(Row[1].vector);
		Row[1] = detail::scale(Row[1].vector, static_cast<T>(1));

		// Next, get Z scale and normalize 3rd row.
		OutScale.vector.z = length(Row[2].vector);
		Row[2] = detail::scale(Row[2].vector, static_cast<T>(1));

		OutRotation.vector.y = asin(-Row[0][2]);
		if (cos(OutRotation.vector.y) != 0)
		{
			OutRotation.vector.x = atan2(Row[1][2], Row[2][2]);
			OutRotation.vector.z = atan2(Row[0][1], Row[0][0]);
		}
		else
		{
			OutRotation.vector.x = atan2(-Row[2][0], Row[1][1]);
			OutRotation.vector.z = 0;
		}

		return true;
	}

	bool DecomposeMatrix(const FMatrix4x4& matrix, FVector& OutLocation, FQuat& OutQuat, FVector& OutScale)
	{
		using namespace glm;
		using T = float;

		FMatrix4x4 LocalMatrix(matrix);

		if (epsilonEqual(LocalMatrix[3][3], static_cast<T>(0), epsilon<T>())) return false;

		if (epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) || epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
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
		OutScale.vector.x = length(Row[0].vector); // v3Length(Row[0]);

		Row[0] = detail::scale(Row[0].vector, static_cast<T>(1));
		// Now, compute Y scale and normalize 2nd row.
		OutScale.vector.y = length(Row[1].vector);
		Row[1] = detail::scale(Row[1].vector, static_cast<T>(1));

		// Next, get Z scale and normalize 3rd row.
		OutScale.vector.z = length(Row[2].vector);
		Row[2] = detail::scale(Row[2].vector, static_cast<T>(1));

		int i, j, k = 0;
		T root, trace = Row[0].vector.x + Row[1].vector.y + Row[2].vector.z;
		if (trace > static_cast<T>(0))
		{
			root = sqrt(trace + static_cast<T>(1.0));
			OutQuat.w = static_cast<T>(0.5) * root;
			root = static_cast<T>(0.5) / root;
			OutQuat.x = root * (Row[1].vector.z - Row[2].vector.y);
			OutQuat.y = root * (Row[2].vector.x - Row[0].vector.z);
			OutQuat.z = root * (Row[0].vector.y - Row[1].vector.x);
		} // End if > 0
		else
		{
			static int Next[3] = {1, 2, 0};
			i = 0;
			if (Row[1].vector.y > Row[0].vector.x) i = 1;
			if (Row[2].vector.z > Row[i][i]) i = 2;
			j = Next[i];
			k = Next[j];

			int off = 0;

			root = sqrt(Row[i][i] - Row[j][j] - Row[k][k] + static_cast<T>(1.0));

			OutQuat[i + off] = static_cast<T>(0.5) * root;
			root = static_cast<T>(0.5) / root;
			OutQuat[j + off] = root * (Row[i][j] + Row[j][i]);
			OutQuat[k + off] = root * (Row[i][k] + Row[k][i]);
			OutQuat.w = root * (Row[j][k] - Row[k][j]);
		}

		return true;
	}

} // namespace Math
