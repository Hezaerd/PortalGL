#include "player.h"
#include "Singleton.h"

PlayerComponent::PlayerComponent(int l)
{
	name = "PlayerComponent";
	life = l;
}

void PlayerComponent::Start(GameObject* gameobject)
{
	portalBlue = SINGLETON.findObjectWithName("bluePortal");
	portalOrange = SINGLETON.findObjectWithName("orangePortal");
	cam = SINGLETON.findObjectWithName("PlayerCam");
	cam->localTransform.setPosition(lm::vec3(0, 1, 0));
	gun = SINGLETON.findObjectWithName("gun");
	gun->localTransform.setPosition(lm::vec3(1, 0.9f, 0.3f));
	SINGLETON.playSound(1, true); //music
}

void PlayerComponent::Update(GameObject* gameobject, float delta)
{
	tpTimer -= delta;
	if (SINGLETON.LMB) {
		if (!hasShootLeft) {
			RayResult result;
			if (Raycast(
				WORLD_PHYSICS.convertVec3ToBtVec3(cam->worldTransform.getPosition()),
				WORLD_PHYSICS.convertVec3ToBtVec3(cam->getFront()),
				100,
				result,
				true))
			{
				if (result.pBody != portalOrange->getComponent<PhysicsComponent>("physics")->getRigidBody()) {
					GameObject* g = SINGLETON.findObjectWithRigidBody(result.pBody);
					if (g != nullptr) {
						if (g->getComponent<CanPlacePortal>("CanPlacePortal") != nullptr) {
							hasShootLeft = true;
							hasPlacedBlue = true;
							placePortal(portalBlue, result);
						}
					}
				}
			}
		}
	}
	else
	{
		hasShootLeft = false;
	}

	if (SINGLETON.RMB) {
		if (!hasShootRight) {
			RayResult result;
			if (Raycast(
				WORLD_PHYSICS.convertVec3ToBtVec3(cam->worldTransform.getPosition()),
				WORLD_PHYSICS.convertVec3ToBtVec3(cam->getFront()),
				100,
				result,
				true))
			{
				if (result.pBody != portalBlue->getComponent<PhysicsComponent>("physics")->getRigidBody()) {
					GameObject* g = SINGLETON.findObjectWithRigidBody(result.pBody);
					if (g != nullptr) {
						if (g->getComponent<CanPlacePortal>("CanPlacePortal") != nullptr) {
							hasShootRight = true;
							hasPlacedOrange = true;
							placePortal(portalOrange, result);
						}
					}
				}
			}
		}
	}
	else
	{
		hasShootRight = false;
	}

	if (SINGLETON.E && !hasSomething) {
		if (!isEpressed) {
			RayResult result;
			if (Raycast(
				WORLD_PHYSICS.convertVec3ToBtVec3(cam->worldTransform.getPosition()),
				WORLD_PHYSICS.convertVec3ToBtVec3(cam->getFront()),
				5,
				result,
				true))
			{
				GameObject* g = SINGLETON.findObjectWithRigidBody(result.pBody);
				if (g != nullptr) {
					if (g->getComponent<Grabable>("Grabable") != nullptr) {
						hasSomething = true;
						isEpressed = true;
						grabObject(g);
					}
					else if (g->getComponent<Button>("Button") != nullptr) {
						g->getComponent<Button>("Button")->Activate();
						isEpressed = true;
					}
				}
			}
		}
	}
	else if (SINGLETON.E && hasSomething)
	{
		if (!isEpressed) {
			isEpressed = true;
			dropObject();
		}
	}
	else
	{
		isEpressed = false;
	}
}

void PlayerComponent::placePortal(GameObject* g, RayResult result)
{
	SINGLETON.playSound(2, false);
	lm::vec3 pos(result.hitPoint.x(), result.hitPoint.y(), result.hitPoint.z());
	g->localTransform.setPosition(pos);
	//solution : 6 case for normals if level is square
	if (result.hitNormal.x() > 0.9f) {
		g->localTransform.setRotation(lm::vec3(0, 180, 0));
		g->getComponent<PhysicsComponent>("physics")->moveRigidBody(
			pos, lm::vec3(0, M_PI, 0));
	}
	else if (result.hitNormal.x() < -0.9f) {
		g->localTransform.setRotation(lm::vec3(0, 0, 0));
		g->getComponent<PhysicsComponent>("physics")->moveRigidBody(
			lm::vec3(result.hitPoint.x(), result.hitPoint.y(), result.hitPoint.z()), lm::vec3(0, 0, 0));
	}
	else if (result.hitNormal.z() > 0.9f) {
		g->localTransform.setRotation(lm::vec3(0, -90, 0));
		g->getComponent<PhysicsComponent>("physics")->moveRigidBody(
			lm::vec3(result.hitPoint.x(), result.hitPoint.y(), result.hitPoint.z()), lm::vec3(0, -M_PI / 2, 0));
	}
	else if (result.hitNormal.z() < -0.9f) {
		g->localTransform.setRotation(lm::vec3(0, 90, 0));
		g->getComponent<PhysicsComponent>("physics")->moveRigidBody(
			lm::vec3(result.hitPoint.x(), result.hitPoint.y(), result.hitPoint.z()), lm::vec3(0, M_PI / 2, 0));
	}
}

void PlayerComponent::grabObject(GameObject* g)
{
	SINGLETON.playSound(6, false);
	g->removeComponent("physics");
	g->setParent(gun);
	g->localTransform.setPosition(lm::vec3::zero);
	grab = g;
}

void PlayerComponent::dropObject()
{
	grab->setParent(nullptr);
	grab->addComponent(new PhysicsComponent(PhysicsComponent::FLAGS::DYNAMICS, lm::vec3(1.f, 1.f, 1.f), 1.f, grab, false));
	hasSomething = false;
}