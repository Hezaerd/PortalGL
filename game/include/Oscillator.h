#pragma once
#include "GameObject.h"

class Oscillator : public Component
{
public:

	// The direction can be :
	// (1,0,0) : X
	// (0,1,0) : Y
	// (0,0,1) : Z
	lm::vec3 direction_ = lm::vec3(0, 0, 0);

	lm::vec3 originalPosition_ = lm::vec3(0, 0, 0);
	float timer_ = 0;
	float periode_ = 0;
	float speed_ = 0;

	Oscillator(lm::vec3 direction, lm::vec3 originalPosition, float periode, float speed) {
		name = "Oscillator";
		originalPosition_ = originalPosition;
		direction_ = direction;
		periode_ = periode;
		speed_ = speed;
	}

	void Update(GameObject* gameObject, float delta) override {
		timer_ += delta / periode_;
		float sinusoidal = sin(timer_ * speed_);
		lm::vec3 newPose (originalPosition_['x'] + sinusoidal * direction_['x'], originalPosition_['y'] + sinusoidal * direction_['y'], originalPosition_['z'] + sinusoidal * direction_['z']);
		gameObject->localTransform.setPosition(newPose);
		gameObject->getComponent<PhysicsComponent>("physics")->moveRigidBody(newPose, lm::vec3 (0,0,0));
	}

	rapidjson::Value getJsonObj(rapidjson::Document::AllocatorType& alloc) override {
		rapidjson::Value c(rapidjson::kObjectType);
		c.AddMember("name", rapidjson::StringRef(name.c_str()), alloc);
		c.AddMember("periode", periode_, alloc);
		c.AddMember("speed", speed_, alloc);
		c.AddMember("direction.x", direction_['x'], alloc);
		c.AddMember("direction.y", direction_['y'], alloc);
		c.AddMember("direction.z", direction_['z'], alloc);
		return c;
	}
};