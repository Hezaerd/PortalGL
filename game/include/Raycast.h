#include "worldphysics.h"

struct RayResult {
	btRigidBody* pBody;
	btVector3 hitPoint;
	btVector3 hitNormal;
};

bool Raycast(const btVector3& startPosition, const btVector3& direction, float distance, RayResult& output, bool kinematics = false);

bool PickingRaycast(const btVector3& startPosition, const btVector3& direction, float distance, RayResult& output);