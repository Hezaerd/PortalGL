#pragma once
#include "GameObject.h"
#include "btBulletDynamicsCommon.h"
#include "WorldPhysics.h"
#include <set>

#include "PhysicsComponent.h"

typedef std::pair<const btRigidBody*, const btRigidBody*> CollisionPair;
typedef std::set<CollisionPair> CollisionPairs;

class CollisionSystem : public PhysicsComponent
{
public:
	CollisionSystem() {}
	void CheckForCollisionEvents();

	CollisionPairs m_pairsLastUpdate;
};
