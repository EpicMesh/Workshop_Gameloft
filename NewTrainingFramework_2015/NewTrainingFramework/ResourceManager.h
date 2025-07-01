#pragma once
#include "SceneObject.h";
#include "Model.h"
#include "Texture.h"
#include "Shader.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>


class ResourceManager
{

private:
	static ResourceManager* rm;
	ResourceManager();

public:
	
	struct ModelResource {
		std::string folder;
		std::string file;
	};

	struct ShaderResource {
		std::string vs;
		std::string fs;
	};

	struct TextureResource {
		std::string type;
		std::string folder;
		std::string file;
		std::string min_filter;
		std::string mag_filter;
		std::string wrap_s;
		std::string wrap_t;
	};

	std::map<int, ModelResource> models;
	std::map<int, ShaderResource> shaders;
	std::map<int, TextureResource> textures;

	std::map<int, Model*> modelsLoaded;
	std::map<int, Shader*> shadersLoaded;
	std::map<int, Texture*> texturesLoaded;

	void parseXML(const char*& xmlFilePath);
	static ResourceManager* getInstance();


	Model* addModel(int id, const std::vector<Vertex>& vertices, const std::vector<unsigned short>& indices);
	Model* getModel(const int& id);
	Texture* getTexture(const int& id);
	Shader* getShader(const int& id);

	~ResourceManager();
};

