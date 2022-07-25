#pragma once
#include "GameObject.h"
#include "Mat4/Mat4.h"
#include "Vec3/Vec3.h"
#include "PhysicsComponent.h"

namespace LowRenderer
{
	const double static degreesToRadians(const double deg)
	{
		return deg * (M_PI / HALF_CIRCLE);
	}
	const double static radianToDegrees(const double rad)
	{
		return rad * (HALF_CIRCLE / M_PI);
	}

	class Camera : public Component
	{
	private:
		lm::Mat4<float> viewMatrix;
		GameObject* obj;
	public:
		Camera(GameObject* obj) {
			name = "Camera";
			this->obj = obj;
		};

		void Update(GameObject* gameObject, float delta) override;
		const lm::mat4 createViewMatrix();
		const lm::mat4 getViewMatrix() const;

		rapidjson::Value getJsonObj(rapidjson::Document::AllocatorType& alloc) override {
			rapidjson::Value c(rapidjson::kObjectType);
			c.AddMember("name", rapidjson::StringRef(name.c_str()), alloc);
			return c;
		};
	};
}

class rotateWithMouse : public Component
{
public:
	rotateWithMouse(float s, bool editor) {
		name = "rotateWithMouse";
		sensitivity = s;
		isEditor = editor;
	};

	void Update(GameObject* gameObject, float delta) override;

	rapidjson::Value getJsonObj(rapidjson::Document::AllocatorType& alloc) override {
		rapidjson::Value c(rapidjson::kObjectType);
		c.AddMember("name", rapidjson::StringRef(name.c_str()), alloc);
		c.AddMember("sensi", sensitivity, alloc);
		c.AddMember("editor", isEditor, alloc);
		return c;
	};

private:
	float sensitivity = 0;
	bool isEditor = false;
};

class freeMovement : public Component {
public:
	freeMovement(float s) {
		name = "freeMovement";
		speed = s;
	}

	void Update(GameObject* gameObject, float delta) override;

private:
	float speed = 0;
};

class playerMovement : public Component {
public:
	GameObject* camera;
	playerMovement(float s, float f) {
		name = "playerMovement";
		speed = s;
		jumpForce = f;
	}

	void Start(GameObject* gameObject) override;

	void Update(GameObject* gameObject, float delta) override;

	rapidjson::Value getJsonObj(rapidjson::Document::AllocatorType& alloc) override {
		rapidjson::Value c(rapidjson::kObjectType);
		c.AddMember("name", rapidjson::StringRef(name.c_str()), alloc);
		c.AddMember("speed", speed, alloc);
		c.AddMember("force", jumpForce, alloc);
		return c;
	};

private:
	float speed = 0;
	float jumpTimer = 0;
	float jumpForce = 1;
};
