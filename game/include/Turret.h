#pragma once
#include "GameObject.h"

class Turret : public Component
{
public:
	Turret();
	~Turret();

private:
	GameObject* player;
	void Start(GameObject* gameObject)override;
	void Update(GameObject* gameObject, float delta) override;
	void turretShootAtPlayer();

	float distance = 100;
	float rangeForFollowing = 10.0f;
	float rangeForShoot = 5.0f;

	float timer = 4.0f;
	float updateTimer = timer;
};