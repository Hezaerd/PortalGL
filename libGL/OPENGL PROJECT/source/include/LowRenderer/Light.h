#pragma once
#include "LowRenderer/Camera.h"
#include "Resources/Shader.h"
#include "Vec4/Vec4.h"
#include "GameObject.h"

namespace LowRenderer
{
	class Light : public Component
	{
	public:
		float diffuse;
		float ambient;
		float specular;

		lm::vec3 lightColor;

		virtual void sendToShader(Resources::Shader* shader, int i, GameObject* obj) {}

		~Light() = default;
	};

	class PointLight : public Light
	{
	public:
		float constant;
		float linear;
		float quadratic;

		PointLight(lm::vec3 lightColor, float diff, float amb, float spec, float constant = 1.f, float linear = 0.045f, float quadratic = 0.0075f);

		rapidjson::Value getJsonObj(rapidjson::Document::AllocatorType& alloc) override {
			rapidjson::Value c(rapidjson::kObjectType);
			c.AddMember("name", rapidjson::StringRef(name.c_str()), alloc);
			c.AddMember("Colorx", lightColor.X(), alloc);
			c.AddMember("Colory", lightColor.Y(), alloc);
			c.AddMember("Colorz", lightColor.Z(), alloc);
			c.AddMember("ambient", ambient, alloc);
			c.AddMember("diffuse", diffuse, alloc);
			c.AddMember("specular", specular, alloc);
			c.AddMember("constant", constant, alloc);
			c.AddMember("linear", linear, alloc);
			c.AddMember("quadratic", quadratic, alloc);
			return c;
		};

		void sendToShader(Resources::Shader* shader, int i, GameObject* obj) override;
	};

	class DirectionLight : public Light
	{
	public:
		DirectionLight(lm::vec3 lightColor, float diff, float amb, float spec);

		rapidjson::Value getJsonObj(rapidjson::Document::AllocatorType& alloc) override {
			rapidjson::Value c(rapidjson::kObjectType);
			c.AddMember("name", rapidjson::StringRef(name.c_str()), alloc);
			c.AddMember("Colorx", lightColor.X(), alloc);
			c.AddMember("Colory", lightColor.Y(), alloc);
			c.AddMember("Colorz", lightColor.Z(), alloc);
			c.AddMember("ambient", ambient, alloc);
			c.AddMember("diffuse", diffuse, alloc);
			c.AddMember("specular", specular, alloc);
			return c;
		};

		void sendToShader(Resources::Shader* shader, int i, GameObject* obj) override;
	};

	class SpotLight : public Light
	{
	public:
		float constant;
		float linear;
		float quadratic;

		float cutOff;
		float outerCutOff;

		SpotLight(lm::vec3 lightColor, float diff, float amb, float spec, float constant = 1.f, float linear = 0.045f, float quadratic = 0.0075f, float cutOff = 12.5f, float outerCutOff = 15.0f);

		rapidjson::Value getJsonObj(rapidjson::Document::AllocatorType& alloc) override {
			rapidjson::Value c(rapidjson::kObjectType);
			c.AddMember("name", rapidjson::StringRef(name.c_str()), alloc);
			c.AddMember("Colorx", lightColor.X(), alloc);
			c.AddMember("Colory", lightColor.Y(), alloc);
			c.AddMember("Colorz", lightColor.Z(), alloc);
			c.AddMember("ambient", ambient, alloc);
			c.AddMember("diffuse", diffuse, alloc);
			c.AddMember("specular", specular, alloc);
			c.AddMember("constant", constant, alloc);
			c.AddMember("linear", linear, alloc);
			c.AddMember("quadratic", quadratic, alloc);
			c.AddMember("cutOff", cutOff, alloc);
			c.AddMember("outerCutOff", outerCutOff, alloc);
			return c;
		};

		void sendToShader(Resources::Shader* shader, int i, GameObject* obj) override;
	};
}
