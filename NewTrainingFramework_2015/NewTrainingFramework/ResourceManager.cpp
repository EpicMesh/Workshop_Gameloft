#include "stdafx.h"

#include "../RapidXML/rapidxml.hpp"
#include "../RapidXML/rapidxml_utils.hpp"
#include "ResourceManager.h"

ResourceManager::ResourceManager(){};

void ResourceManager::parseXML(const char*& xmlFilePath)
{
	rapidxml::file<> file(xmlFilePath);
	char* buffer = file.data();

	rapidxml::xml_document<> doc;
	doc.parse<0>(buffer);

	rapidxml::xml_node<>* resourceManager = doc.first_node("resourceManager");
	if (!resourceManager) {
		std::cerr << "Node not found. " << std::endl;
		return;
	}


	rapidxml::xml_node<>* modelsNode = resourceManager->first_node("models");
	if (modelsNode) {
		rapidxml::xml_node<>* folderNode = modelsNode->first_node("folder");
		
		while (folderNode) {
			std::string folderName = folderNode->first_attribute("path")->value();
			rapidxml::xml_node<>* modelNode = folderNode->first_node("model");
			while (modelNode) {
				ModelResource model;
				int id = std::stoi(modelNode->first_attribute("id")->value());
				model.file = folderName;
				model.file.append(modelNode->first_node("file")->value());
				models.insert({id,model});
				modelNode = modelNode->next_sibling("model");
			}
			folderNode = folderNode->next_sibling("folder");
		}
	}


	rapidxml::xml_node<>* shadersNode = resourceManager->first_node("shaders");
	if (shadersNode) {
		rapidxml::xml_node<>* folderNode = shadersNode->first_node("folder");
		while (folderNode) {
			std::string folderName = folderNode->first_attribute("path")->value();
			rapidxml::xml_node<>* shaderNode = folderNode->first_node("shader");
			while (shaderNode) {
				ShaderResource shader;
				int id = std::stoi(shaderNode->first_attribute("id")->value());
				shader.vs = folderName;
				shader.vs.append(shaderNode->first_node("vs")->value());
				shader.fs = folderName;
				shader.fs.append(shaderNode->first_node("fs")->value());
				shaders.insert({ id,shader });
				shaderNode = shaderNode->next_sibling("shader");
			}
			folderNode = folderNode->next_sibling("folder");
		}
	}


	rapidxml::xml_node<>* texturesNode = resourceManager->first_node("textures");
	if (texturesNode) {
		rapidxml::xml_node<>* folderNode = texturesNode->first_node("folder");
		while (folderNode) {
			std::string folderName = folderNode->first_attribute("path")->value();
			rapidxml::xml_node<>* textureNode = folderNode->first_node("texture");
			while (textureNode) {
				TextureResource texture;
				int id = std::stoi(textureNode->first_attribute("id")->value());
				texture.type = textureNode->first_attribute("type")->value();
				texture.file = folderName;
				texture.file.append(textureNode->first_node("file")->value());
				texture.min_filter = textureNode->first_node("min_filter")->value();
				texture.mag_filter = textureNode->first_node("mag_filter")->value();
				texture.wrap_s = textureNode->first_node("wrap_s")->value();
				texture.wrap_t = textureNode->first_node("wrap_t")->value();
				textures.insert({id,texture});
				textureNode = textureNode->next_sibling("texture");
			}
			folderNode = folderNode->next_sibling("folder");
		}
	}
};

ResourceManager* ResourceManager::rm = NULL;

ResourceManager* ResourceManager::getInstance()
{
	if (!rm)
	{
		rm = new ResourceManager();
	}
	return rm;
}

Model* ResourceManager::getModel(const int& id)
{	
	auto itLoaded = modelsLoaded.find(id);
	if (itLoaded != modelsLoaded.end())
	{
		return itLoaded->second;
	}

	auto it = models.find(id);

	if (it != models.end()) {

		Model* model = new Model();
		std::string path = it->second.file;
		model->Load(path);
		modelsLoaded.insert({ id,model });
		return model;
	}
	return nullptr;
}

Texture* ResourceManager::getTexture(const int& id)
{	
	auto itLoaded = texturesLoaded.find(id);
	if (itLoaded != texturesLoaded.end())
	{
		return itLoaded->second;
	}

	auto it = textures.find(id);

	if (it != textures.end()) {

		Texture* texture = new Texture();
		const char* path = it->second.file.c_str();
		texture->Load(path,id);

		return texture;
	}
	return nullptr;
}

Shader* ResourceManager::getShader(const int& id)
{	
	auto itLoaded = shadersLoaded.find(id);
	if (itLoaded != shadersLoaded.end())
	{
		return itLoaded->second;
	}

	auto it = shaders.find(id);

	if (it != shaders.end()) {

		Shader* shader = new Shader();
		char* pathVS = strdup(it->second.vs.c_str());
		char* pathFS = strdup(it->second.fs.c_str());
		shader->Load(pathVS,pathFS);
		free(pathVS);
		free(pathFS);
		return shader;
	}
	return nullptr;
}

Model* ResourceManager::addModel(int id, const std::vector<Vertex>& vertices, const std::vector<unsigned short>& indices) {
	
	auto itLoaded = modelsLoaded.find(id);
	if (itLoaded != modelsLoaded.end())
	{
		return itLoaded->second;
	}

		Model* model = new Model();
		model->LoadFromData(vertices, indices);
		modelsLoaded.insert({ id,model });
		return model;

	
}


ResourceManager::~ResourceManager() {};