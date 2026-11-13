#include <Physics/Include/PhysData.h>

FVector CoreEngine::Physic::ConverBulletToCustomVector(const btVector3& Vec)
{
	return FVector(Vec.x(), Vec.y(), Vec.x());
}
