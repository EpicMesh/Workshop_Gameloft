#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h"

#include <string>
#include <vector>


class Texture
{
private:
	static Texture* tr;
	Texture();

public:
	GLuint idTexture;
	std::string type;
	static Texture* getInstance();
	void Load(const char*& path,int id);
	~Texture();
	friend class ResourceManager;
};

