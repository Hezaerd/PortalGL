#include "Resources/Application.h"

GameObject* Singleton::findObjectWithName(std::string str)
{
	for (int i = 0; i < app->gameObjects.size(); i++)
	{
		if (app->gameObjects[i]->name == str) {
			return app->gameObjects[i];
		}
	}
	return nullptr;
}

GameObject* Singleton::findObjectWithRigidBody(btRigidBody* rgbd)
{
	for (int i = 0; i < app->gameObjects.size(); i++)
	{
		if (app->gameObjects[i]->getComponent<PhysicsComponent>("physics")) {
			if (app->gameObjects[i]->getComponent<PhysicsComponent>("physics")->getRigidBody() == rgbd) {
				return app->gameObjects[i];
			}
		}
	}
	return nullptr;
}