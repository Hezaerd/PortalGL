#include "LowRenderer/Light.h"
#include "Singleton.h"

void LowRenderer::SpotLight::sendToShader(Resources::Shader* shader, int i, GameObject* obj)
{
	std::string str = "spotLights[" + std::to_string(i) + "]";
	std::string temp = str;

	shader->setVec3f((str.append(".position")).c_str(), obj->worldTransform.getPosition());
	str = temp;
	shader->setVec3f((str.append(".direction")).c_str(), obj->getFront());
	str = temp;
	shader->setVec3f((str.append(".color")).c_str(), this->lightColor);
	str = temp;
	shader->setFloat((str.append(".ambient")).c_str(), this->ambient);
	str = temp;
	shader->setFloat((str.append(".diffuse")).c_str(), this->diffuse);
	str = temp;
	shader->setFloat((str.append(".specular")).c_str(), this->specular);
	str = temp;
	shader->setFloat(str.append(".constant"), this->constant);
	str = temp;
	shader->setFloat(str.append(".linear"), this->linear);
	str = temp;
	shader->setFloat(str.append(".quadratic"), this->quadratic);
	str = temp;
	shader->setFloat((str.append(".cutOff")).c_str(), static_cast<float>(cos(degreesToRadians(this->cutOff))));
	str = temp;
	shader->setFloat((str.append(".outerCutOff")).c_str(), static_cast<float>(cos(degreesToRadians(this->outerCutOff))));
}

LowRenderer::SpotLight::SpotLight(lm::vec3 lightColor, float diff, float amb, float spec, float constant, float linear, float quadratic, float cutOff, float outerCutOff)
{
	name = "spotLight";
	this->lightColor = lightColor;
	this->diffuse = diff;
	this->ambient = amb;
	this->specular = spec;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
	this->cutOff = cutOff;
	this->outerCutOff = outerCutOff;
}

LowRenderer::PointLight::PointLight(lm::vec3 lightColor, float diff, float amb, float spec, float constant, float linear, float quadratic)
{
	name = "pointLight";
	this->diffuse = diff;
	this->lightColor = lightColor;
	this->ambient = amb;
	this->specular = spec;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

void LowRenderer::PointLight::sendToShader(Resources::Shader* shader, int i, GameObject* obj)
{
	// point light 1
	std::string str = "pointLights[" + std::to_string(i) + "]";
	std::string temp = str;
	shader->setVec3f((str.append(".position")).c_str(), obj->worldTransform.getPosition());
	str = temp;
	shader->setVec3f((str.append(".color")).c_str(), this->lightColor);
	str = temp;
	shader->setFloat((str.append(".ambient")).c_str(), this->ambient);
	str = temp;
	shader->setFloat((str.append(".diffuse")).c_str(), this->diffuse);
	str = temp;
	shader->setFloat((str.append(".specular")).c_str(), this->specular);
	str = temp;
	shader->setFloat(str.append(".constant"), this->constant);
	str = temp;
	shader->setFloat(str.append(".linear"), this->linear);
	str = temp;
	shader->setFloat(str.append(".quadratic"), this->quadratic);
}

LowRenderer::DirectionLight::DirectionLight(lm::vec3 lightColor, float diff, float amb, float spec)
{
	name = "dirLight";
	this->lightColor = lightColor;
	this->ambient = amb;
	this->diffuse = diff;
	this->specular = spec;
}

void LowRenderer::DirectionLight::sendToShader(Resources::Shader* shader, int i, GameObject* obj)
{
	std::string str = "dirLights[" + std::to_string(i) + "]";
	std::string temp = str;
	shader->setVec3f((str.append(".direction").c_str()), obj->getFront());
	str = temp;
	shader->setVec3f((str.append(".color").c_str()), this->lightColor);
	str = temp;
	shader->setFloat((str.append(".ambient").c_str()), this->ambient);
	str = temp;
	shader->setFloat((str.append(".diffuse").c_str()), this->diffuse);
	str = temp;
	shader->setFloat((str.append(".specular").c_str()), this->specular);
	str = temp;
}