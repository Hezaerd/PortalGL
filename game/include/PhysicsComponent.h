#pragma once
#include "Singleton.h"
#include "MotionState.h"
#include "GameObject.h"
#include "player.h"
#include <vector>

class PhysicsComponent : public Component
{
public:
	bool checkCollision = false;
	std::vector<btRigidBody*> currentCol;
	__event void onCollisionEnter(btRigidBody* body1, btRigidBody* body2);
	__event void onCollisionExit(btRigidBody* body1, btRigidBody* body2);
	void CheckForCollisionEvents();

	enum FLAGS { DYNAMICS = 0, STATIC, KINEMATIC };
	PhysicsComponent(int flags, const lm::vec3& halfextent, float mass, GameObject* object, bool sleep = true,
		bool rigidBody = true, int colisionChanel = 0);
	PhysicsComponent();
	~PhysicsComponent() override;

	void Update(GameObject* gameobject, float delta) override;

	// accessors
	btCollisionShape* GetShape() { return m_pShape; }
	MotionState* GetMotionState() { return m_pMotionState; }

	void GetTransform(btScalar* transform) {
		if (m_pMotionState) m_pMotionState->GetWorldTransform(transform);
	}

	btVector3 GetColor() { return m_color; }

	void SetKinematic(bool isKinematic);

	rapidjson::Value getJsonObj(rapidjson::Document::AllocatorType& alloc) override {
		rapidjson::Value c(rapidjson::kObjectType);
		c.AddMember("name", rapidjson::StringRef(name.c_str()), alloc);

		c.AddMember("flag", flag, alloc);

		c.AddMember("mass", mass, alloc);

		c.AddMember("halfExtentX", halfextent.X(), alloc);
		c.AddMember("halfExtentY", halfextent.Y(), alloc);
		c.AddMember("halfExtentZ", halfextent.Z(), alloc);
		c.AddMember("rigidBody", rigidBody, alloc);
		c.AddMember("sleep", sleep, alloc);
		return c;
	}

	void moveRigidBody(vec3 const& newPosition, vec3 const& newOrientation);
	btRigidBody* getRigidBody();

	void Start(GameObject* gameobject) override {
		GetMotionState()->setGraphMat(&gameobject->localTransform.Matrix);
	}

private:
	float mass;
	int flag;
	bool rigidBody = true;
	bool sleep = true;
	lm::vec3 halfextent;
	btVector3 initialPosition;
	btQuaternion initialRotation;
	lm::vec3 scale;
	btCollisionShape* m_pShape;
	btRigidBody* m_pBody;

	MotionState* m_pMotionState;
	btVector3      m_color;
};

class PortalCollision : public Component
{
public:
	GameObject* otherPortal = nullptr;
	GameObject* player = nullptr;
	GameObject* playerCam = nullptr;
	PlayerComponent* playerComp = nullptr;
	PortalCollision() {
		name = "portalCollision";
	}
	void Start(GameObject* gameObject) override;

	void portalCollisionEnter(btRigidBody* body1, btRigidBody* body2);
	void portalCollisionExit(btRigidBody* body1, btRigidBody* body2) {
	};
};

class CanPlacePortal : public Component {
public:
	CanPlacePortal() {
		name = "CanPlacePortal";
	}
};

class Grabable : public Component {
public:
	Grabable() {
		name = "Grabable";
	}
};
