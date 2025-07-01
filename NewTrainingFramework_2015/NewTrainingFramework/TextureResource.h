#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h"

class TextureResource
{
private:
	TextureResource* tr;

public:
	GLuint textureId;
	TextureResource(TextureResource* resource);
	~TextureResource();
	void Load();
};

