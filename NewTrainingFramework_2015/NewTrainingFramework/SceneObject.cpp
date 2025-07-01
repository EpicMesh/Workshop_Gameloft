#include "stdafx.h"
#include "SceneObject.h"
#include "iostream"
#include "ResourceManager.h"
#include "SceneManager.h"


#define MAX_TEXTURES 5

SceneObject::SceneObject()
{

}

SceneObject::~SceneObject()
{

}

void SceneObject::Draw()
{	
	
	
	glUseProgram(this->shader->program);

	glBindBuffer(GL_ARRAY_BUFFER, this->model->vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->model->iboId);

	if (this->isGenerated)
	{
		
		sendSpecificData();
		glDrawElements(GL_TRIANGLES, this->model->nrIndici, GL_UNSIGNED_SHORT, 0);

	}
	else {

		sendCommonData();
		glDrawElements(GL_TRIANGLES, this->model->nrIndici, GL_UNSIGNED_SHORT, 0);
		
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void SceneObject::sendCommonData() {
	
	bool hasCubeMap = false;

	for (auto& texture : textures) {
		if (texture.second->type == "cube") {
			hasCubeMap = true;
		}
	}
	
	glUniform1i(this->shader->useColorUniform, hasColor);
	glUniform1i(this->shader->hasCubeMapUniform, hasCubeMap ? 1 : 0);


	int textureUnit = 0;
	for (auto& texture : textures) {
		if (textureUnit >= MAX_TEXTURES) break;

		glActiveTexture(GL_TEXTURE0 + textureUnit);

		if (texture.second->type == "cube") {
			glBindTexture(GL_TEXTURE_CUBE_MAP, texture.second->idTexture);
			glUniform1i(this->shader->cubeMapTextureUniform, textureUnit);
			
		}
		else if(texture.second->type == "2d"){
			glBindTexture(GL_TEXTURE_2D, texture.second->idTexture);
			glUniform1i(this->shader->textureUniform[textureUnit], textureUnit);
		}

		textureUnit++;
	}
	

	float smallRadius = SceneManager::getInstance()->smallRadius;
	float hugeRadius = SceneManager::getInstance()->hugeRadius;
	Vector3 fogColor = SceneManager::getInstance()->fogColor;
	
	Matrix translationMatrix;
	Matrix scaleMatrix;
	Matrix rotationMatrix1;
	Matrix rotationMatrix2;
	Matrix rotationMatrix3;
	Matrix rotationMatrixTotal;
	
	translationMatrix.SetTranslation(position);
	scaleMatrix.SetScale(scale);
	rotationMatrix1.SetRotationX(radians(rotation[0]));
	rotationMatrix2.SetRotationY(radians(rotation[1]));
	rotationMatrix3.SetRotationZ(radians(rotation[2]));
	
	

	rotationMatrixTotal = rotationMatrix1 * rotationMatrix2 * rotationMatrix3;

	Matrix mvp;
	mvp = scaleMatrix * rotationMatrixTotal * translationMatrix * camera.viewMatrix * camera.perspectiveMatrix;
	

	if (this->shader->matrixUniform != -1)
	{
		glUniformMatrix4fv(this->shader->matrixUniform, 1, GL_FALSE, (float*)mvp.m);
	}

	if (this->shader->positionAttribute != -1)
	{
		glEnableVertexAttribArray(this->shader->positionAttribute);
		glVertexAttribPointer(this->shader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (this->shader->normAttribute != -1)
	{
		glEnableVertexAttribArray(this->shader->normAttribute);
		glVertexAttribPointer(this->shader->normAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2*sizeof(Vector3)));
	}

	if (this->shader->colorAttribute != -1)
	{
		glEnableVertexAttribArray(this->shader->colorAttribute);
		glVertexAttribPointer(this->shader->colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vector3));
	}

	GLint colorUniform = glGetUniformLocation(this->shader->program, "u_color");
	if (colorUniform != -1) {
		glUniform3f(colorUniform, color.x, color.y, color.z);
	}


	if (this->shader->uvAttribute != -1) {
		glEnableVertexAttribArray(this->shader->uvAttribute);
		glVertexAttribPointer(this->shader->uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5 * sizeof(Vector3)));
	}


	//Fog

	Matrix modelMatrix;
	modelMatrix = scaleMatrix * rotationMatrixTotal * translationMatrix;

	if (this->shader->modelMatrixUniform != -1)
	{
		glUniformMatrix4fv(this->shader->modelMatrixUniform, 1, GL_FALSE, (float*)modelMatrix.m);
	}

	Vector3 cameraPosition = camera.getPosition();
	if (this->shader->camPosition != -1)
	{
		glUniform3f(this->shader->camPosition, cameraPosition.x, cameraPosition.y, cameraPosition.z);
	}


	if (this->shader->smallRUniform != -1) {
		glUniform1f(this->shader->smallRUniform, smallRadius);
	}

	if (this->shader->hugeRUniform != -1) {
		glUniform1f(this->shader->hugeRUniform, hugeRadius);
	}
	

	if (this->shader->fogColorUniform != -1)
	{
		glUniform3f(this->shader->fogColorUniform, fogColor.x, fogColor.y, fogColor.z);
	}

	if (this->shader->ambientalColorUniform != -1)
	{	
		Vector3 ambientalColor = Vector3(SceneManager::getInstance()->ambientLight.color.x, SceneManager::getInstance()->ambientLight.color.y, SceneManager::getInstance()->ambientLight.color.z);
		glUniform3f(this->shader->ambientalColorUniform, ambientalColor.x, ambientalColor.y, ambientalColor.z);
	}

	if (this->shader->ambientalRatioUniform != -1)
	{
		glUniform1f(this->shader->ambientalRatioUniform, SceneManager::getInstance()->ambientLight.ratio);
	}

	for (int i = 0; i < idLights.size();i++)
	{
		for (auto& lightPair : SceneManager::getInstance()->lights)
		{
			if (lightPair.first == idLights[i])
			{	

				if (this->shader->lightPosUniform != -1)
				{
					std::vector<float> lightPosition = lightPair.second->getLightPos();
					glUniform3f(this->shader->lightPosUniform, lightPosition.at(0), lightPosition.at(1), lightPosition.at(2));
				}

				if (this->shader->diffuseColorUniform != -1)
				{
					std::vector<float> diffuseColor = lightPair.second->getDiffuseColor();
					glUniform3f(this->shader->diffuseColorUniform, diffuseColor.at(0), diffuseColor.at(1), diffuseColor.at(2));
				}

				if (this->shader->specularColorUniform != -1)
				{
					std::vector<float> specularColor = lightPair.second->getSpecularColor();
					glUniform3f(this->shader->specularColorUniform, specularColor.at(0), specularColor.at(1), specularColor.at(2));
				}

				if (this->shader->typeUniform != -1)
				{
					if (lightPair.second->getType() == "point")
					{
						glUniform1i(this->shader->typeUniform, 0);
					}
				}
			}
		}
	}
	
}

void SceneObject::sendSpecificData() 
{	
	float smallRadius = SceneManager::getInstance()->smallRadius;
	float hugeRadius = SceneManager::getInstance()->hugeRadius;
	Vector3 fogColor = SceneManager::getInstance()->fogColor;

	Matrix translationMatrix;
	Matrix scaleMatrix;
	Matrix rotationMatrix1;
	Matrix rotationMatrix2;
	Matrix rotationMatrix3;
	Matrix rotationMatrixTotal;

	translationMatrix.SetTranslation(position);
	scaleMatrix.SetScale(scale);
	rotationMatrix1.SetRotationX(radians(rotation[0]));
	rotationMatrix2.SetRotationY(radians(rotation[1]));
	rotationMatrix3.SetRotationZ(radians(rotation[2]));

	rotationMatrixTotal = rotationMatrix1 * rotationMatrix2 * rotationMatrix3;

	Matrix mvp;
	mvp = scaleMatrix * rotationMatrixTotal * translationMatrix * camera.viewMatrix * camera.perspectiveMatrix;
	

	if (this->shader->matrixUniform != -1)
	{
		glUniformMatrix4fv(this->shader->matrixUniform, 1, GL_FALSE, (float*)mvp.m);
	}
	else {
		std::cerr << "Eroare: Uniforma matrixUniform este invalida!\n";
	}

	int i = 1;
	for (auto& texture : textures) {
		if (i >= MAX_TEXTURES) break;
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, texture.second->idTexture);
		glUniform1i(this->shader->textureUniform[i-1], i);
		i++;
	}
	glActiveTexture(GL_TEXTURE0);
	
	GLint colorUniform = glGetUniformLocation(this->shader->program, "u_color");
	if (colorUniform != -1) {
		glUniform3f(colorUniform, 1.0, 0.0, 0.0);
	}


	GLint uHeightUniform = glGetUniformLocation(this->shader->program, "u_height");
	if (uHeightUniform != -1)
	{
		glUniform3f(uHeightUniform, 1000.0, 20.0, 5.0);
	}

	GLint uNUniform = glGetUniformLocation(this->shader->program, "u_N");
	if (uNUniform != -1)
	{
		glUniform1f(uNUniform, 100);
	}

	GLint offsetUniform = glGetUniformLocation(this->shader->program, "u_offset");
	if (offsetUniform != -1)
	{
		glUniform2f(offsetUniform, offset.x, offset.y);
	}

	if (this->shader->positionAttribute != -1)
	{
		glEnableVertexAttribArray(this->shader->positionAttribute);
		glVertexAttribPointer(this->shader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (this->shader->normAttribute != -1)
	{
		glEnableVertexAttribArray(this->shader->normAttribute);
		glVertexAttribPointer(this->shader->normAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(Vector3)));
	}

	if (this->shader->uvAttribute != -1) {
		glEnableVertexAttribArray(this->shader->uvAttribute);
		glVertexAttribPointer(this->shader->uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5 * sizeof(Vector3)));
	}

	//Fog
	Matrix modelMatrix;
	modelMatrix = scaleMatrix * rotationMatrixTotal * translationMatrix;

	if (this->shader->modelMatrixUniform != -1)
	{
		glUniformMatrix4fv(this->shader->modelMatrixUniform, 1, GL_FALSE, (float*)modelMatrix.m);
	}

	Vector3 cameraPosition = camera.getPosition();
	if (this->shader->camPosition != -1)
	{
		glUniform3f(this->shader->camPosition, cameraPosition.x, cameraPosition.y, cameraPosition.z);
	}


	if (this->shader->smallRUniform != -1) {
		glUniform1f(this->shader->smallRUniform, smallRadius);
	}

	if (this->shader->hugeRUniform != -1) {
		glUniform1f(this->shader->hugeRUniform, hugeRadius);
	}

	if (this->shader->fogColorUniform != -1)
	{
		glUniform3f(this->shader->fogColorUniform, fogColor.x, fogColor.y, fogColor.z);
	}

	

	
}

void SceneObject::Update(float deltaTime)
{
	
}

