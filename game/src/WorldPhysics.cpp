#include "WorldPhysics.h"

#include <vector>

WorldPhysics::WorldPhysics()
{
	collisionConfiguration = new btDefaultCollisionConfiguration();

	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	overlappingPairCache = new btDbvtBroadphase();

	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

WorldPhysics::WorldPhysics(btVector3 vector3)
{
	collisionConfiguration = new btDefaultCollisionConfiguration();

	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	overlappingPairCache = new btDbvtBroadphase();

	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(vector3);
}

void WorldPhysics::addRigidbody(btRigidBody* rigid_body)
{
	this->dynamicsWorld->addRigidBody(rigid_body);
}

void WorldPhysics::addCollisionObj(btCollisionObject* collision_object)
{
	this->dynamicsWorld->addCollisionObject(collision_object);
}

void WorldPhysics::removeRigidbody(btRigidBody* rigid_body)
{
	this->dynamicsWorld->removeRigidBody(rigid_body);
}

//get all rigid bodies from dynamics world
void WorldPhysics::removeAllRigidBodies()
{
	std::vector<btRigidBody*> rigid_bodies;

	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);

		if (body && body->getMotionState())
		{
			removeRigidbody(body);
		}
	}
}

btVector3 WorldPhysics::convertVec3ToBtVec3(const lm::vec3& vec3)
{
	return btVector3(vec3.X(), vec3.Y(), vec3.Z());
}

WorldPhysics::~WorldPhysics()
{
	delete dynamicsWorld;
	delete solver;
	delete overlappingPairCache;
	delete dispatcher;
	delete collisionConfiguration;
}