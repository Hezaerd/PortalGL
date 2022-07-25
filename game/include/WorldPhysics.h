#pragma once
#include "btBulletDynamicsCommon.h"
#include "Vec3/Vec3.h"

#include "GameObject.h"

class WorldPhysics
{
private:
	btVector3 gravity = btVector3(0, -10, 0);
	btDefaultCollisionConfiguration* collisionConfiguration = nullptr;
	btCollisionDispatcher* dispatcher = nullptr;
	btBroadphaseInterface* overlappingPairCache = nullptr;
	btSequentialImpulseConstraintSolver* solver = nullptr;
	btDiscreteDynamicsWorld* dynamicsWorld = nullptr;
public:
	//constructor / destructor
	WorldPhysics();
	WorldPhysics(btVector3 vector3);
	~WorldPhysics();

	//accessor
	//get dispatcher
	btCollisionDispatcher* getDispatcher() { return dispatcher; }
	//get overlapping pair cache
	btBroadphaseInterface* getOverlappingPairCache() { return overlappingPairCache; }
	//get solver
	btSequentialImpulseConstraintSolver* getSolver() { return solver; }

	btDiscreteDynamicsWorld* getWorld() { return dynamicsWorld; }
	//set
	void setGravity();

	//func
	void addRigidbody(btRigidBody* rigid_body);
	void addCollisionObj(btCollisionObject* collision_object);
	void removeRigidbody(btRigidBody* rigid_body);
	void removeAllRigidBodies();

	//Convertion
	btVector3 convertVec3ToBtVec3(const lm::vec3& vec3);

	//collision

	static WorldPhysics& getInstance()
	{
		static WorldPhysics instance;
		return instance;
	}
};

#define WORLD_PHYSICS WorldPhysics::getInstance()
