#include <Math/includes/Quat.h>

namespace Math
{
	FVector ToEuler(const FQuat& quat)
	{
		return glm::eulerAngles(quat);
	}
} // namespace Math
