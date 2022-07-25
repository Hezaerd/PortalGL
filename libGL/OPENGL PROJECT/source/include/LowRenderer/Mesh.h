#pragma once

#include "GameObject.h"
#include "Resources/Texture.h"
#include "Resources/Model.h"
#include "Camera.h"
#include "Vec3/Vec3.h"

namespace LowRenderer
{
	class Mesh : public Component
	{
	private:
		Resources::Model* model;
		Resources::Texture* texture = nullptr;
		std::string mPath;
		std::string tPath;
	public:
		Mesh(std::string modelPath, std::string texturePath = "");
		lm::Mat4<float> modelMat;
		lm::Mat4<float> projectionMat;
		lm::Mat4<float> viewMat;
		lm::Mat4<float> mvp;

		void updateUniform(Resources::Shader* shader, Camera* cam);
		lm::mat4 calculateMVP(lm::Mat4<float> view);

		void Update(GameObject* gameObject, float delta) override;

		rapidjson::Value getJsonObj(rapidjson::Document::AllocatorType& alloc) override {
			rapidjson::Value c(rapidjson::kObjectType);
			c.AddMember("name", rapidjson::StringRef(name.c_str()), alloc);
			c.AddMember("modelPath", rapidjson::StringRef(mPath.c_str()), alloc);
			c.AddMember("texturePath", rapidjson::StringRef(tPath.c_str()), alloc);
			return c;
		};
	};
}
