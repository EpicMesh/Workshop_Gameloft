#pragma once
#include "Model.h"
#include "Shader.h"
#include "Light.h"
#include "Texture.h"
#include "Camera.h"
#include "Vertex.h"
#include <map>
#include <vector>




class SceneObject
{
	public:
		SceneObject();
		~SceneObject();
		int id;
		GLfloat position[3], rotation[3], scale[3];
		Model* model = nullptr;
		Shader* shader = nullptr;
		Texture* texture = nullptr;
		Vector3 color;
		int hasColor = 0;
		std::string type;
		std::string name;
		std::map<int, Texture*> textures;
		std::vector<int> idLights;
		bool depthTest;
		Camera camera;
		int currentLight;

		//Terrain
		bool isGenerated = false;
		bool isUploadedToGPU = false;
		std::vector<Vertex> vertices;
		std::vector<GLushort> indices;
		GLuint vboId = 0;
		GLuint iboId = 0;
		void generateModel(int N, float D);
		void generateIndices(int N);
		void uploadToGPU();


		void setCamera(Camera& cam) { camera = cam; };
		virtual void Draw()=0 ;
		void sendCommonData();
		void sendSpecificData();
		virtual void Update(float deltaTime)=0;
		void setActiveTexture(int textureId);

		void moveTerrain();
		Vector2 offset{0,0};


		float radians(float degree)
		{
			return (degree * (3.14159 / 180));
		};
};

