#pragma once
#include "GameObject.h"
#include "Raycast.h"
#include "Button.h"

class  PlayerComponent : public Component
{
public:
	int life = 3;
	float tpTimer = 0;
	GameObject* portalBlue = nullptr;
	GameObject* portalOrange = nullptr;
	GameObject* cam = nullptr;
	GameObject* gun = nullptr;
	GameObject* grab = nullptr;
	bool hasShootRight = false;
	bool hasShootLeft = false;
	bool isEpressed = false;
	bool hasSomething = false;
	bool hasPlacedOrange = false;
	bool hasPlacedBlue = false;
	PlayerComponent(int l);
	void Start(GameObject* gameobject) override;
	void Update(GameObject* gameobject, float delta) override;
	void placePortal(GameObject* g, RayResult result);
	void grabObject(GameObject* g);
	void dropObject();
	rapidjson::Value getJsonObj(rapidjson::Document::AllocatorType& alloc) {
		rapidjson::Value c(rapidjson::kObjectType);
		c.AddMember("name", rapidjson::StringRef(name.c_str()), alloc);
		c.AddMember("life", life, alloc);
		return c;
	};
};
