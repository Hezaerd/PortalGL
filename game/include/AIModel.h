#pragma once

// libGL
#include "AIMesh.h"
#include "Resources/Shader.h"

// stb_image
#include <stb_image.h>

// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace Resources;

class Model
{
public:
	std::vector<ATexture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;

	Model(const std::string& path);

	void draw(Shader& shader);

private:
	void loadModel(const std::string& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<ATexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typename);
};

unsigned int TextureFromFile(const std::string& path);