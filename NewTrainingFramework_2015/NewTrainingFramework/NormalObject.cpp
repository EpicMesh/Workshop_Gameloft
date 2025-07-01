#include "stdafx.h"
#include "NormalObject.h"
#include <iostream>

NormalObject::NormalObject() {
    type = "normal";
}

NormalObject::~NormalObject() {}

void NormalObject::Draw() {
    glUseProgram(this->shader->program);

    glBindBuffer(GL_ARRAY_BUFFER, this->model->vboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->model->iboId);

    if (isGenerated) {
        
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

}

void NormalObject::moveTerrain(float deltaTime)
{
    Vector3 cameraPos = camera.getPosition();
    GLfloat terrainPos[3];

    if (isGenerated)
    {
        std::cout << isGenerated << std::endl;
        terrainPos[0] = position[0];
        terrainPos[1] = position[1];
        terrainPos[2] = position[2];

        float dx = terrainPos[0] - cameraPos.x;
        float dz = terrainPos[2] - cameraPos.z;

        const float cellSize = 110.0f;


        if(fabs(dx) >= cellSize)
        {
            if (dx > 0)
            {
                position[0] -= cellSize;
                offset.x -= 1;
            }
            else
            {
                position[0] += cellSize;
                offset.x += 1;

            }
        }

        if (fabs(dz) >= cellSize) 
        {
            if (dz > 0)
            {
                position[2] -= cellSize;
                offset.y -= 1;
            }
            else
            {
                position[2] += cellSize;
                offset.y += 1;
            }
        }
        //std::cout << offset[0] << " " << offset[1] << std::endl;
        //std::cout << terrainPos[0] << " " << terrainPos[1] << " " << terrainPos[2] << std::endl;
        //std::cout << cameraPos.x << " " << cameraPos.y << " " << cameraPos.z << std::endl;
        //std::cout << dx << " " << dz << std::endl;
    }
}

void NormalObject::Update(float deltaTime) {
    moveTerrain(deltaTime);
}