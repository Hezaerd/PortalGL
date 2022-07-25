#include "LowRenderer/Camera.h"

#include "player.h"
#include "Singleton.h"

void LowRenderer::Camera::Update(GameObject* gameObject, float delta)
{
	createViewMatrix();
}

const lm::mat4 LowRenderer::Camera::createViewMatrix()
{
	lm::mat4 tmp;
	this->viewMatrix = tmp.lookAt(obj->worldTransform.getPosition(), obj->worldTransform.getPosition() + obj->getFront(), obj->getUp());
	return this->viewMatrix;
}

const lm::mat4 LowRenderer::Camera::getViewMatrix() const
{
	return this->viewMatrix;
}

void freeMovement::Update(GameObject* gameObject, float delta) {
	if (SINGLETON.W)
		gameObject->localTransform.translate(gameObject->getFront() * SINGLETON.delta * this->speed);

	if (SINGLETON.S)
		gameObject->localTransform.translate(-gameObject->getFront() * SINGLETON.delta * this->speed);

	if (SINGLETON.A)
		gameObject->localTransform.translate(-gameObject->getRight() * SINGLETON.delta * this->speed);

	if (SINGLETON.D)
		gameObject->localTransform.translate(gameObject->getRight() * SINGLETON.delta * this->speed);
}

void rotateWithMouse::Update(GameObject* gameObject, float delta) {
	if (!SINGLETON.RMB && isEditor)
		return;

	float Ry = static_cast<float>(-SINGLETON.mouseOffSetX * this->sensitivity * SINGLETON.timescale);
	float Rz = static_cast<float>(-SINGLETON.mouseOffSetY * this->sensitivity * SINGLETON.timescale);

	gameObject->localTransform.addRotation(lm::vec3(0, Ry, Rz));

	if (gameObject->localTransform.getRotation().Z() > 80.f)
	{
		gameObject->localTransform.setRotation(lm::vec3(gameObject->localTransform.getRotation().X(), gameObject->localTransform.getRotation().Y(), 80.f));
	}
	else if (gameObject->localTransform.getRotation().Z() < -80.f)
	{
		gameObject->localTransform.setRotation(lm::vec3(gameObject->localTransform.getRotation().X(), gameObject->localTransform.getRotation().Y(), -80.f));
	}
};

void playerMovement::Start(GameObject* gameObject)
{
	camera = SINGLETON.findObjectWithName("PlayerCam");
}

void playerMovement::Update(GameObject* gameObject, float delta)
{
	jumpTimer -= delta;
	lm::vec3 temp;
	if (SINGLETON.W) {
		temp += camera->getFront();
	}

	if (SINGLETON.S) {
		temp -= camera->getFront();
	}

	if (SINGLETON.A) {
		temp -= camera->getRight();
	}

	if (SINGLETON.D) {
		temp += camera->getRight();
	}

	temp.Y() = 0;

	if (SINGLETON.SPACE && jumpTimer < 0) {
		temp += lm::vec3::up * jumpForce;
		jumpTimer = 2;
	}

	if (temp != lm::vec3::zero) {
		temp *= this->speed * delta * 100;
		temp += lm::vec3(0, gameObject->getComponent<PhysicsComponent>("physics")->getRigidBody()->getLinearVelocity().getY(), 0);
		gameObject->getComponent<PhysicsComponent>("physics")->getRigidBody()->setLinearVelocity(btVector3(temp.X(), temp.Y(), temp.Z()));
	}
}