#include "stdafx.h"
#include "Shader.h"
#include "Vertex.h"

#include <fstream>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#define MAX_TEXTURES 4

Shader::Shader() {
	for (int i = 0; i < MAX_TEXTURES; i++) {
		textureUniform[i] = -1;  
	}
};

Shader* Shader::sr = NULL;

Shader* Shader::getInstance()
{
	if (!sr)
	{
		sr = new Shader();
	}
	return sr;
}

int Shader::Load(char* fileVertexShader, char* fileFragmentShader) {
	
	vertexShader = esLoadShader(GL_VERTEX_SHADER, fileVertexShader);

	if (vertexShader == 0)
		return -1;

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fileFragmentShader);

	if (fragmentShader == 0)
	{
		glDeleteShader(vertexShader);
		return -2;
	}

	

	program = esLoadProgram(vertexShader, fragmentShader);
	if (program == 0) return -3;
	//finding location of uniforms / attributes
	positionAttribute = glGetAttribLocation(program, "a_posL");
	normAttribute = glGetAttribLocation(program, "a_norm");
	colorAttribute = glGetAttribLocation(program, "a_color");
	matrixUniform = glGetUniformLocation(program, "uMVP");
	uvAttribute = glGetAttribLocation(program, "a_uv");
	useColorUniform = glGetUniformLocation(program, "useColorUniform");

	uNUniform = glGetUniformLocation(program, "u_N");  
	uHeightUniform = glGetUniformLocation(program, "u_height");
	offsetUniform = glGetUniformLocation(program, "u_offset");

	//Fog
	smallRUniform = glGetUniformLocation(program, "smallR");
	hugeRUniform = glGetUniformLocation(program, "hugeR");
	modelMatrixUniform = glGetUniformLocation(program, "modelMatrix");
	camPosition = glGetUniformLocation(program, "camPos");
	fogColorUniform = glGetUniformLocation(program, "fogColor");

	//Fire
	u_DispMax_Uniform = glGetUniformLocation(program, "u_DispMax");
	u_Time_Uniform = glGetUniformLocation(program, "u_Time");

	cubeMapTextureUniform = glGetUniformLocation(program, "u_cube_texture");
	hasCubeMapUniform = glGetUniformLocation(program, "hasCubeMap");

	for (int i = 0; i < MAX_TEXTURES; i++) {
		std::string textureUniformName = "u_texture_" + std::to_string(i);
		textureUniform[i] = glGetUniformLocation(program, textureUniformName.c_str());
		if (textureUniform[i] == -1) {
			std::cerr << "Warning: Uniform " << textureUniformName << " not found in shader!\n";
		}
	}

	//Lights
	ambientalColorUniform = glGetUniformLocation(program, "c_amb");
	ambientalRatioUniform = glGetUniformLocation(program, "ratio");

	lightPosUniform = glGetUniformLocation(program, "lightPos");
	diffuseColorUniform = glGetUniformLocation(program, "c_luminaDiff");
	specularColorUniform = glGetUniformLocation(program, "c_luminaSpec");
	typeUniform = glGetUniformLocation(program, "type");

	return 0;
}

void Shader::sendShaderData(Matrix mvp)
{
	if (positionAttribute != -1)
	{
		glEnableVertexAttribArray(positionAttribute);
		glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (colorAttribute != -1)
	{
		glEnableVertexAttribArray(colorAttribute);
		glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vector3));
	}
	if (matrixUniform != -1)
	{
		glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, (float*)mvp.m);
	}

	if (uvAttribute != -1) {

		glEnableVertexAttribArray(uvAttribute);

		glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5 * sizeof(Vector3)));
	}
	

}


Shader::~Shader() {

	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
