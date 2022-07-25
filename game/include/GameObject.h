#pragma once
#include "Transfrom.h"
#include "Mat4/mat4.h"
#include "Vec3/vec3.h"
#include <map>
#include "rapidjson/document.h"

class PhysicsComponent;
class Component;

class GameObject
{
public:
	int id;
	Transfrom localTransform;
	Transfrom worldTransform;
	GameObject* parent = nullptr;
	lm::mat4 Matrix;
	std::map<std::string, Component*> components;
	bool isStatic = false;

	std::string name;

	GameObject(std::string name);
	~GameObject();

	void Start();
	void Update();
	void UpdateMatrix();
	void UpdateComponent();
	void UpdateRender();
	void UpdateBulletTranform(MotionState* motion);
	void updateGrapMat();
	void setName(const std::string& newName);
	std::string getName();
	lm::vec3 getFront();
	lm::vec3 getRight();
	lm::vec3 getUp();
	void addComponent(Component* comp);
	template<typename T>
	T* getComponent(std::string key)
	{
		auto it = components.find(key);
		if (it == components.end())
		{
			return nullptr;
		}

		return static_cast <T*>(it->second);
	}
	void removeComponent(std::string name);
	void setParent(GameObject* p = nullptr);
};

class Component
{
public:

	std::string name = "";
	virtual void Update(GameObject* gameObject, float delta) {};
	virtual void Start(GameObject* gameObject) {};
	virtual ~Component() {};
	virtual rapidjson::Value getJsonObj(rapidjson::Document::AllocatorType& alloc) {
		rapidjson::Value c(rapidjson::kObjectType);
		c.AddMember("name", rapidjson::StringRef(name.c_str()), alloc);
		return c;
	};
};

class testComponent : public Component
{
public:
	float speed;

	testComponent(float s) {
		name = "Test";
		speed = s;
	}

	void Update(GameObject* gameObject, float delta) override {
		gameObject->localTransform.addRotation(lm::vec3(0, delta * speed, 0));
	}

	rapidjson::Value getJsonObj(rapidjson::Document::AllocatorType& alloc) override {
		rapidjson::Value c(rapidjson::kObjectType);
		c.AddMember("name", rapidjson::StringRef(name.c_str()), alloc);
		c.AddMember("speed", speed, alloc);
		return c;
	}
};

class testComponent2 : public Component
{
public:
	float t = 0;

	testComponent2(float s) {
		name = "Test2";
		t = s;
	}

	void Update(GameObject* gameObject, float delta) override {
		gameObject->localTransform.translate(lm::vec3(0, t * delta, 0));
	}

	rapidjson::Value getJsonObj(rapidjson::Document::AllocatorType& alloc) override {
		rapidjson::Value c(rapidjson::kObjectType);
		c.AddMember("t", t, alloc);
		return c;
	}
};