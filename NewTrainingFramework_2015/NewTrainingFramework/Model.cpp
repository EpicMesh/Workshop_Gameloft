#include "stdafx.h"
#include "Vertex.h"
#include "Model.h"

#include <fstream>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <sstream>


Model::Model(){};

Model* Model::mr = NULL;

Model* Model::getInstance()
{
    if (!mr)
    {
        mr = new Model();
    }
    return mr;
}

void Model::Load(std::string path) {
	std::vector<Vertex> vertecsi;
	std::vector<unsigned short> indici;
	
	

	std::ifstream file(path);
	if (!file.is_open()) {
		return;
	}

	std::string line;
	size_t numVertices = 0;

	std::getline(file, line);
	if (line.find("NrVertices:") != std::string::npos) {
		std::istringstream iss(line);
		std::string label;
		iss >> label >> numVertices;
	}
	//nrVertecsi = numVertices;

	// Read vertex data
	for (size_t i = 0; i < numVertices; ++i) {
		if (!std::getline(file, line)) break;

		Vertex vertex;
		sscanf(line.c_str(),
			"%*d. pos:[%f, %f, %f]; norm:[%f, %f, %f]; binorm:[%f, %f, %f]; tgt:[%f, %f, %f]; uv:[%f, %f];",
			&vertex.pos.x, &vertex.pos.y, &vertex.pos.z,
			&vertex.norm.x, &vertex.norm.y, &vertex.norm.z,
			&vertex.binorm.x, &vertex.binorm.y, &vertex.binorm.z,
			&vertex.tgt.x, &vertex.tgt.y, &vertex.tgt.z,
			&vertex.uv.x, &vertex.uv.y);
		vertecsi.push_back(vertex);
	}


	while (std::getline(file, line)) {
		unsigned short a, b, c;
		if (sscanf(line.c_str(), "%*d. %hu, %hu, %hu", &a, &b, &c) == 3) {
			indici.push_back(a);
			indici.push_back(b);
			indici.push_back(c);
		}
	}
	//nrIndici = indici.size();

	file.close();

	LoadFromData(vertecsi, indici);

	/*glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertecsi.size(), vertecsi.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indici.size() * sizeof(unsigned short), indici.data(), GL_STATIC_DRAW);*/

}

void Model::LoadFromData(const std::vector<Vertex>& vertices, const std::vector<unsigned short>& indices) {

	nrVertecsi = vertices.size();
	nrIndici = indices.size();

	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * nrVertecsi, vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nrIndici * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);

}

Model::~Model() {
    // Eliberarea bufferelor
    if (vboId) glDeleteBuffers(1, &vboId);
    if (iboId) glDeleteBuffers(1, &iboId);
    if (wiredIbold) glDeleteBuffers(1, &wiredIbold);
}
