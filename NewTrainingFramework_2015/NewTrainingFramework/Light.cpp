#include "stdafx.h"
#include "Light.h"


Light::Light() {};

Light::Light(int idL,std::vector<float> lightPosL, std::vector<float> diffuseColorL, std::vector<float> specularColorL, std::string typeL) {
	id = idL;

	diffuseColor[0] = diffuseColorL[0];
	diffuseColor[1] = diffuseColorL[1];
	diffuseColor[2] = diffuseColor[2];

	specularColor[0] = specularColorL[0];
	specularColor[1] = specularColorL[1];
	specularColor[2] = specularColorL[2];
		
	lightPos[0] = lightPosL[0];
	lightPos[1] = lightPosL[1];
	lightPos[2] = lightPosL[2];

	type = typeL;
};

Light::~Light() {};