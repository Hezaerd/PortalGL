#include "PhysicsComponent.h"

void PhysicsComponent::CheckForCollisionEvents()
{
	if (!checkCollision) {
		return;
	}
	std::vector<btRigidBody*> colVec;
	for (int i = 0; i < WORLD_PHYSICS.getDispatcher()->getNumManifolds(); ++i) {
		// get the manifold
		btPersistentManifold* pManifold = WORLD_PHYSICS.getDispatcher()->getManifoldByIndexInternal(i);

		// get the two bodies that are involved in the collision
		if (pManifold->getNumContacts() > 0) {
			// get the two rigid bodies involved in the collision
			const btRigidBody* pBody0 = static_cast<const btRigidBody*>(pManifold->getBody0());
			const btRigidBody* pBody1 = static_cast<const btRigidBody*>(pManifold->getBody1());

			if (m_pBody == pBody0) {
				//if not in current collision
				if (std::find(currentCol.begin(), currentCol.end(), (btRigidBody*)pBody1) == currentCol.end()) {
					__raise onCollisionEnter(m_pBody, (btRigidBody*)pBody1);
					currentCol.push_back((btRigidBody*)pBody1);
				}
				colVec.push_back((btRigidBody*)pBody1);
			}
			if (m_pBody == pBody1) {
				//if not in current collision
				if (std::find(currentCol.begin(), currentCol.end(), (btRigidBody*)pBody0) == currentCol.end()) {
					__raise onCollisionEnter(m_pBody, (btRigidBody*)pBody0);
					currentCol.push_back((btRigidBody*)pBody0);
				}
				colVec.push_back((btRigidBody*)pBody0);
			}
		}
	}
	for (int j = 0; j < currentCol.size(); j++)
	{
		if (std::find(colVec.begin(), colVec.end(), currentCol[j]) == colVec.end()) {
			__raise onCollisionExit(m_pBody, currentCol[j]);
			currentCol.erase(currentCol.begin() + j);
		}
	}
}

PhysicsComponent::PhysicsComponent(int flags, const lm::vec3& halfextent, float mass, GameObject* object, bool sleep,
	bool rigidBody, int collisionChanel)
{
	this->flag = flags;
	this->mass = mass;
	name = "physics";
	m_pShape = new btBoxShape(WORLD_PHYSICS.convertVec3ToBtVec3(halfextent));
	this->halfextent = halfextent;
	this->rigidBody = rigidBody;
	this->initialPosition = btVector3(object->localTransform.getPosition()[0], object->localTransform.getPosition()[1], object->localTransform.getPosition()[2]);
	this->sleep = sleep;
	btQuaternion rot;
	rot.setEuler(object->localTransform.getRotation()[0] * M_PI / 180, object->localTransform.getRotation()[1] * M_PI / 180, object->localTransform.getRotation()[2] * M_PI / 180);
	this->initialRotation = rot;
	this->scale = object->localTransform.getScale();
	switch (flags)
	{
	case STATIC:
		m_color = btVector3(0, 1, 0);
		break;
	default:
		m_color = btVector3(1, 1, 1);
		break;
	}

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(this->initialPosition);
	transform.setRotation(this->initialRotation);

	m_pMotionState = new MotionState(transform);

	btVector3 localInertia(0, 0, 0);

	if (this->mass != 0.0f)
		this->m_pShape->calculateLocalInertia(this->mass, localInertia);
	if (rigidBody)
	{
		btRigidBody::btRigidBodyConstructionInfo cInfo(this->mass, m_pMotionState, m_pShape, localInertia);

		// create the rigid body
		m_pBody = new btRigidBody(cInfo);
		m_pBody->setCollisionFlags(flags);
		if (!sleep)
		{
			m_pBody->setActivationState(DISABLE_DEACTIVATION);
		}
	}
	m_pBody->collisionChannel = collisionChanel;
	m_pShape->setLocalScaling(btVector3(this->scale.X(), this->scale.Y(), this->scale.Z()));

	WORLD_PHYSICS.addRigidbody(m_pBody);
}

PhysicsComponent::PhysicsComponent()
{
	//init all variables
	this->mass = 0.0f;
	this->flag = 0;
	this->m_pShape = nullptr;
	this->m_pBody = nullptr;
	this->m_pMotionState = nullptr;
}

void PhysicsComponent::moveRigidBody(vec3 const& newPosition, vec3 const& newOrientation)
{
	btRigidBody* rb = getRigidBody();
	if (rb == nullptr) return;
	WORLD_PHYSICS.getWorld()->removeRigidBody(getRigidBody());

	MotionState* ms = GetMotionState();

	btVector3 position(newPosition.X(), newPosition.Y(), newPosition.Z());
	const btScalar yaw = newOrientation.Y();
	const btScalar pitch = newOrientation.X();
	const btScalar roll = newOrientation.Z();
	btQuaternion orientation(yaw, pitch, roll);

	btTransform initialTransform;

	initialTransform.setOrigin(position);
	initialTransform.setRotation(orientation);

	rb->setWorldTransform(initialTransform);
	ms->setWorldTransform(initialTransform);
	rb->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
	rb->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
	rb->clearForces();

	WORLD_PHYSICS.getWorld()->addRigidBody(rb);
}

btRigidBody* PhysicsComponent::getRigidBody()
{
	return m_pBody;
}

PhysicsComponent::~PhysicsComponent()
{
	WORLD_PHYSICS.removeRigidbody(m_pBody);
	delete m_pBody;
	delete m_pMotionState;
	delete m_pShape;
}

void PhysicsComponent::Update(GameObject* gameobject, float delta)
{
	gameobject->updateGrapMat();
	CheckForCollisionEvents();
}

void PhysicsComponent::SetKinematic(bool isKinematic)
{
	if (m_pBody == nullptr)
		return;

	if (isKinematic)
		m_pBody->setCollisionFlags(m_pBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	else
		m_pBody->setCollisionFlags(m_pBody->getCollisionFlags() & ~(btCollisionObject::CF_KINEMATIC_OBJECT));
}

void PortalCollision::Start(GameObject* gameObject)
{
	PhysicsComponent* t = gameObject->getComponent<PhysicsComponent>("physics");
	if (gameObject->name != "bluePortal") {
		otherPortal = SINGLETON.findObjectWithName("bluePortal");
	}
	else
	{
		otherPortal = SINGLETON.findObjectWithName("orangePortal");
	}
	player = SINGLETON.findObjectWithName("Player");
	playerComp = player->getComponent<PlayerComponent>("PlayerComponent");
	playerCam = SINGLETON.findObjectWithName("PlayerCam");

	if (t != nullptr) {
		t->checkCollision = true;
		__hook(&PhysicsComponent::onCollisionEnter, t, &PortalCollision::portalCollisionEnter);
		__hook(&PhysicsComponent::onCollisionExit, t, &PortalCollision::portalCollisionExit);
	}
}

void PortalCollision::portalCollisionEnter(btRigidBody* body1, btRigidBody* body2)
{
	if (otherPortal && player)
	{
		if (player->getComponent<PhysicsComponent>("physics")->getRigidBody() == body2) {
			if (playerComp->hasPlacedBlue && playerComp->hasPlacedOrange) {
				if (playerComp->tpTimer < 0) {
					SINGLETON.playSound(3, false);
					playerComp->tpTimer = 0.1f;
					lm::vec3 front(otherPortal->getFront());
					player->getComponent<PhysicsComponent>("physics")->moveRigidBody(otherPortal->worldTransform.getPosition() + -front * 2, lm::vec3::zero);
					if (front.X() > 0.9f) {
						playerCam->localTransform.setRotation(lm::vec3(0, 180, 0));
					}
					else if (front.X() < -0.9f) {
						playerCam->localTransform.setRotation(lm::vec3(0, 0, 0));
					}
					else if (front.Z() > 0.9f) {
						playerCam->localTransform.setRotation(lm::vec3(0, 90, 0));
					}
					else if (front.Z() < -0.9f) {
						playerCam->localTransform.setRotation(lm::vec3(0, -90, 0));
					}

					player->getComponent<PhysicsComponent>("physics")->getRigidBody()->setLinearVelocity(-btVector3(front.X(), front.Y(), front.Z()) * 2);
				}
			}
		}
	}
}
