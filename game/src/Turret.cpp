#include "Turret.h"
#include <math.h>
#include "LowRenderer/Camera.h"
#include "player.h"

Turret::Turret()
{
	name = "Turret";
}

Turret::~Turret()
{
	SINGLETON.playSound(7, false); //turret destroyed
}

void Turret::Start(GameObject* gameObject)
{
	player = SINGLETON.findObjectWithName("Player");
}

void Turret::Update(GameObject* gameObject, float delta)
{
	distance = sqrt((player->localTransform.getPosition().X() - gameObject->localTransform.getPosition().X()) * (player->localTransform.getPosition().X() - gameObject->localTransform.getPosition().X()) +
		(player->localTransform.getPosition().Y() - gameObject->localTransform.getPosition().Y()) * (player->localTransform.getPosition().Y() - gameObject->localTransform.getPosition().Y()) +
		(player->localTransform.getPosition().Z() - gameObject->localTransform.getPosition().Z()) * (player->localTransform.getPosition().Z() - gameObject->localTransform.getPosition().Z()));

	timer -= delta;
	if (timer <= 0.0f)
		timer = updateTimer;

	if (distance <= 25.0f)
	{
		float Ry = std::atan2f(player->worldTransform.getPosition().X() - gameObject->worldTransform.getPosition().X(), player->worldTransform.getPosition().Z() - gameObject->worldTransform.getPosition().Z());
		gameObject->localTransform.setRotation(lm::vec3(0, Ry * 180 / M_PI, 0));

		if (distance <= 5.0f)
		{
			if (timer <= 1.0f)
			{
				turretShootAtPlayer();
				timer = 0.0f;
			}
		}
	}
}

void Turret::turretShootAtPlayer()
{
	if (player->getComponent<PlayerComponent>("PlayerComponent")->life > 0)
	{
		player->getComponent<PlayerComponent>("PlayerComponent")->life -= 1;
		std::cout << player->getComponent<PlayerComponent>("PlayerComponent")->life << '\n';
	}
}