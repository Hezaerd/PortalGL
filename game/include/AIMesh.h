#pragma once

#include "Resources/Shader.h"

#include "Vec3/Vec3.h"
#include "Vec2/Vec2.h"

#include <string>
#include <vector>

using namespace Resources;

struct Vertex
{
	lm::Vec3<float> Position;
	lm::Vec3<float> Normal;
	lm::Vec2<float> TexCoords;
};

struct ATexture
{
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<ATexture> m_textures;
	unsigned int VAO;

	// Constructeur
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<ATexture> textures);

	void draw(Shader& shader);

private:
	unsigned int VBO;
	unsigned int EBO;

	void setupMesh();
};