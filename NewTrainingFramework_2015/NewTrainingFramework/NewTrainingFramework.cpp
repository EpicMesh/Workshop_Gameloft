// NewTrainingFramework.cpp : Defines the entry point for the console application.
//



#include "stdafx.h"
#include <conio.h>
#include<iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "Vertex.h"
#include "Shader.h"
#include "Globals.h"
#include "Camera.h"
#define PI 3.14159265359
#include "Obiect.h"
#include "NewTrainingFramework.h"
#include "Model.h"
#include "ResourceManager.h"
#include "SceneManager.h"
// if you use STL, please include this line AFTER all other include
#include "../Utilities/utilities.h"

GLuint lineVbold;
float angle{0};
float step{0.0002};
float totalTime {0};


Vector3 position = Vector3(0, 0, -1);
Vector3 target = Vector3(0, 0, 0);
Vector3 up = Vector3(0, 1, 0);
GLfloat fov = 0.785398;
GLfloat nearPlane = 0.2;
GLfloat farPlane = 1000;
GLfloat moveSpeed = 100;
GLfloat rotateSpeed = 0.1;
//Camera camera(position, target, up, fov, nearPlane, farPlane, moveSpeed, rotateSpeed);


const char* xmlPath = "D:/POLITEHNICA/Anul_1/WorkshopGameloft/proiect_2015/NewResourcesPacket/XML/resourceManager.xml";
ResourceManager* resourceManager = nullptr;

/*Model* crocomodel;
Texture* crocotexture;
int idModelCroco = 1;
int textureIdCroco = 1;

Model* busmodel;
Texture* bustexture;
int idModelBus = 6;
int textureIdBus = 6;

Shader* shader;
int shaderId = 10;*/

const char* scenexmlPath = "D:/POLITEHNICA/Anul_1/WorkshopGameloft/proiect_2015/NewResourcesPacket/XML/sceneManager.xml";

SceneManager* sceneManager = nullptr;

int Init ( ESContext *esContext )
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor ( sceneManager->backgroundColor.r, sceneManager->backgroundColor.g, sceneManager->backgroundColor.b, 0.0f );

	glEnable(GL_DEPTH_TEST);

	//line data
	/*Vertex lineVerticesData[2];
	lineVerticesData[0].pos.x = 0; lineVerticesData[0].pos.y = 1; lineVerticesData[0].pos.z = 0;
	lineVerticesData[1].pos.x = 0; lineVerticesData[1].pos.y = -1; lineVerticesData[1].pos.z = 0;

	//triangle data (heap)
	Vertex verticesData[6];

	verticesData[0].pos.x =  -0.5f;  verticesData[0].pos.y =  -0.5f;  verticesData[0].pos.z =  0.0f;
	verticesData[1].pos.x = -0.5f;  verticesData[1].pos.y = 0.5f;  verticesData[1].pos.z =  0.0f;
	verticesData[2].pos.x =  0.5f;  verticesData[2].pos.y = 0.5f;  verticesData[2].pos.z =  0.0f;

	verticesData[3].pos.x = -0.5f;  verticesData[3].pos.y = -0.5f;  verticesData[3].pos.z = 0.0f;
	verticesData[4].pos.x = 0.5f;  verticesData[4].pos.y = -0.5f;  verticesData[4].pos.z = 0.0f;
	verticesData[5].pos.x = 0.5f;  verticesData[5].pos.y = 0.5f;  verticesData[5].pos.z = 0.0f;
	
	//VertexColors
	verticesData[0].color.x = 1; verticesData[0].color.y = 0; verticesData[0].color.z = 0;
	verticesData[1].color.x = 0; verticesData[1].color.y = 1; verticesData[1].color.z = 0;
	verticesData[2].color.x = 0; verticesData[2].color.y = 0; verticesData[2].color.z = 1;

	verticesData[3].color.x = 1; verticesData[3].color.y = 0; verticesData[3].color.z = 0;
	verticesData[4].color.x = 1; verticesData[4].color.y = 1; verticesData[4].color.z = 0;
	verticesData[5].color.x = 0; verticesData[5].color.y = 0; verticesData[5].color.z = 1;

	//newBuffer
	glGenBuffers(1, &lineVbold);
	glBindBuffer(GL_ARRAY_BUFFER, lineVbold);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineVerticesData), lineVerticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//buffer object
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	lineShader.Init("../Resources/Shaders/LineShaderVS.vs", "../Resources/Shaders/LineShaderFS.fs");
	//creation of shaders and program */

	/*glGenBuffers(1, &modelVbold);
	glBindBuffer(GL_ARRAY_BUFFER, modelVbold);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertecsi.size(), vertecsi.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &modelIbold);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelIbold);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indici.size() * sizeof(unsigned short), indici.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/
	
	resourceManager = resourceManager->getInstance();
	resourceManager->parseXML(xmlPath);

	sceneManager->Init(scenexmlPath);
	sceneManager->setUpCamera();
	
	//crocomodel = resourceManager->getModel(idModelCroco);
	//crocotexture = resourceManager->getTexture(textureIdCroco);

	
	//busmodel = resourceManager->getModel(idModelBus);
	//bustexture = resourceManager->getTexture(textureIdBus);

	//shader = resourceManager->getShader(shaderId);
	
	

	return 0;
	
	//return modelShader.Init("../Resources/Shaders/modelShaderVS.vs", "../Resources/Shaders/modelShaderFS.fs");

	//return myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");

}




void Draw ( ESContext *esContext )
{
	
	Matrix mRotation;
	mRotation.SetRotationZ(angle);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	/*glUseProgram(myShaders.program);
	

	glBindBuffer(GL_ARRAY_BUFFER, vboId);


	
	if(myShaders.positionAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders.positionAttribute);
		glVertexAttribPointer(myShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (myShaders.colorAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders.colorAttribute);
		glVertexAttribPointer(myShaders.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) sizeof(Vector3));
	}

	if (myShaders.matrixUniform != -1)
	{
		glUniformMatrix4fv(myShaders.matrixUniform,1,GL_FALSE, (float*) mvp.m);
	}
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//Lines
	glUseProgram(lineShader.program);
	glBindBuffer(GL_ARRAY_BUFFER, lineVbold);
	if (lineShader.positionAttribute != -1)
	{
		glEnableVertexAttribArray(lineShader.positionAttribute);
		glVertexAttribPointer(lineShader.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (lineShader.colorAttribute != -1)
	{
		glEnableVertexAttribArray(lineShader.colorAttribute);
		glVertexAttribPointer(lineShader.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vector3));
	}

	if (lineShader.matrixUniform != -1)
	{
		glUniformMatrix4fv(lineShader.matrixUniform, 1, GL_FALSE, (float*)mvp.m);
	}

	
	glDrawArrays(GL_LINES, 0, 2);
	*/

	sceneManager->Draw();
	
	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{

	sceneManager->Update(deltaTime);
	
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{	
	sceneManager->handleKeyInput(key,bIsPressed);
}

void Mouse(ESContext* esContext, MouseNameSpace::MouseButtons btn, MouseNameSpace::MouseEvents event, int x, int y)
{
	std::cout << btn << event;
	std::cout << x << " " << y << std::endl;

	sceneManager->handleMouseInput(btn, event, x, y);
	
}

void CleanUp()
{

}

int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	ESContext esContext;
	esInitContext(&esContext);

	sceneManager = sceneManager->getInstance();
	sceneManager->InitWindow(scenexmlPath);

	esCreateWindow(&esContext, sceneManager->gameName.c_str(), sceneManager->screenSize.width, sceneManager->screenSize.height, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if (Init(&esContext) != 0)
		return 0;
	
	esRegisterDrawFunc(&esContext, Draw);
	esRegisterUpdateFunc(&esContext, Update);
	esRegisterKeyFunc(&esContext, Key);
	esRegisterMouseFunc(&esContext, Mouse);
	esMainLoop(&esContext);

	//releasing OpenGL resources
	CleanUp();


	printf("Press any key...\n");
	_getch();

	
	return 0;
}

