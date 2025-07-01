#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h"

class ShaderResource
{
private:
	ShaderResource* sr;
public:
	GLuint programId;
	ShaderResource(ShaderResource* resource);
	~ShaderResource();
	void Load();
};

