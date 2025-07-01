#include "stdafx.h"
#include "Texture.h"
#include "Vertex.h"
#include "ResourceManager.h"

#include <fstream>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <sstream>


Texture::Texture() {};

Texture* Texture::tr = NULL;

Texture* Texture::getInstance()
{
	if (!tr)
	{
		tr = new Texture();
	}
	return tr;
}

void Texture::Load(const char*& path, int id) {
	int width, height, bpp;
	char* pixelArray = nullptr;
	pixelArray = LoadTGA(path, &width, &height, &bpp);

	ResourceManager* rm = ResourceManager::getInstance();
	auto& textureResource = rm->textures[id];
	type = rm->textures[id].type;

	glGenTextures(1, &idTexture);

	if (type == "2d") {
		glBindTexture(GL_TEXTURE_2D, idTexture);

		if (bpp == 24)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelArray);
		}
		else if (bpp == 32)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelArray);
		}

		if (textureResource.min_filter == "LINEAR")
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		if (textureResource.mag_filter == "LINEAR")
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		if (textureResource.wrap_s == "CLAMP_TO_EDGE")
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		}

		if (textureResource.wrap_t == "CLAMP_TO_EDGE")
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		if (textureResource.wrap_s == "REPEAT")
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		}

		if (textureResource.wrap_t == "REPEAT")
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else if (type == "cube")
	{

		glBindTexture(GL_TEXTURE_CUBE_MAP, idTexture);

		int tileWidth = width / 4;
		int tileHeight = height / 3;

		struct FaceCoords {
			int x, y;
		};

		FaceCoords faces[6] = {
			{2 * tileWidth,  tileHeight},  // Right (+X)
			{0 * tileWidth,  tileHeight},  // Left (-X)
			{1 * tileWidth,  0 * tileHeight},  // Top (+Y)
			{1 * tileWidth,  2 * tileHeight},  // Bottom (-Y)
			{1 * tileWidth,  tileHeight},  // Front (+Z)
			{3 * tileWidth,  tileHeight}  // Back (-Z)
		};

		GLenum targets[6] = {
			GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		};

		for (int i = 0; i < 6; i++) {
			int x = faces[i].x;
			int y = faces[i].y;

			unsigned char* faceData = new unsigned char[tileWidth * tileHeight * (bpp / 8)];

			for (int row = 0; row < tileHeight; row++) {
				memcpy(
					faceData + row * tileWidth * (bpp / 8),
					pixelArray + ((y + row) * width + x) * (bpp / 8),
					tileWidth * (bpp / 8)
				);
			}


			glTexImage2D(targets[i], 0, (bpp == 24) ? GL_RGB : GL_RGBA,
				tileWidth, tileHeight, 0,
				(bpp == 24) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, faceData);
			int error = glGetError();
			delete[] faceData;
		}


		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		 int error = glGetError();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		 error = glGetError();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		 error = glGetError();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		 error = glGetError();

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	}
}


Texture::~Texture() {
	// Eliberarea bufferelor
	if (idTexture) glDeleteBuffers(1, &idTexture);
}
