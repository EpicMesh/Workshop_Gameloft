#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h"
#include "Math.h"

#include <string>
#include<vector>


class Shader
{
private:
	static Shader* sr;
	Shader();

public:
	GLuint program, vertexShader, fragmentShader;
	char fileVS[260];
	char fileFS[260];
	GLint positionAttribute;
	GLint normAttribute;
	GLint colorAttribute;
	GLint matrixUniform;
	GLint textureUniform[5];
	GLint useColorUniform;
	GLint uvAttribute;
	GLint uNUniform;
	GLint uHeightUniform;
	GLint offsetUniform;
	
	//Fog
	GLint smallRUniform;
	GLint hugeRUniform;
	GLint modelMatrixUniform;
	GLint camPosition;
	GLint fogColorUniform;

	GLint cubeMapTextureUniform;

	//Fire
	GLint u_DispMax_Uniform;
	GLint u_Time_Uniform;

	GLint hasCubeMapUniform;


	//Lights
	GLint ambientalColorUniform;
	GLint ambientalRatioUniform;

	GLint lightPosUniform;
	GLint diffuseColorUniform;
	GLint specularColorUniform;
	GLint typeUniform;

	static Shader* getInstance();
	int Load(char* fileVertexShader, char* fileFragmentShader);
	void sendShaderData(Matrix mvp);
	~Shader();
	void sendCubeMapTexture(GLuint textureID);
	friend class ResourceManager;
};

