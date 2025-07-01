#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h"

#include <string>
#include<vector>
#include"Vertex.h"


class Model
{ 
  private:
	  static Model* mr;
	  Model();

  public:
	  int nrIndici, nrIndiciWired,nrVertecsi;
	  GLuint vboId, iboId, wiredIbold;
	  static Model* getInstance();
	  void Load(std::string path);
	  void LoadFromData(const std::vector<Vertex>& vertices, const std::vector<unsigned short>& indices);
	  ~Model();
	  friend class ResourceManager;
};

