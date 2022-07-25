#include "Raycast.h"

bool Raycast(const btVector3& startPosition, const btVector3& direction, float distance, RayResult& output, bool includeStatic)
{
	if (!WORLD_PHYSICS.getWorld())
		return false;

	btVector3 rayFrom = startPosition;
	btVector3 rayTo = startPosition + (direction * distance);

	WORLD_PHYSICS.getWorld()->getDebugDrawer()->drawLine(rayFrom, rayTo, btVector4(0, 0, 1, 1));

	btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);

	WORLD_PHYSICS.getWorld()->rayTest(rayFrom, rayTo, rayCallback);

	if (rayCallback.hasHit())
	{
		btRigidBody* pBody = (btRigidBody*)btRigidBody::upcast(rayCallback.m_collisionObject);
		if (!pBody)
			return false;

		if (!includeStatic)
			if (pBody->isStaticObject() || pBody->isKinematicObject())
				return false;

		output.pBody = pBody;
		output.hitPoint = rayCallback.m_hitPointWorld;
		output.hitNormal = rayCallback.m_hitNormalWorld;
		return true;
	}

	return false;
}

bool PickingRaycast(const btVector3& startPosition, const btVector3& direction, float distance, RayResult& output)
{
	if (!WORLD_PHYSICS.getWorld())
		return false;

	btVector3 rayTo = startPosition + (direction * distance);
	btVector3 rayFrom = startPosition;

	WORLD_PHYSICS.getWorld()->getDebugDrawer()->drawLine(rayFrom, rayTo, btVector4(0, 0, 1, 1));

	btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);

	WORLD_PHYSICS.getWorld()->rayTest(rayFrom, rayTo, rayCallback);

	if (rayCallback.hasHit())
	{
		btRigidBody* pBody = (btRigidBody*)btRigidBody::upcast(rayCallback.m_collisionObject);
		if (!pBody)
			return false;

		if (pBody->isStaticObject() || pBody->isKinematicObject())
			return false;

		output.pBody = pBody;
		output.hitPoint = rayCallback.m_hitPointWorld;
		output.hitNormal = rayCallback.m_hitNormalWorld;
		return true;
	}

	return false;
}