#include "stdafx.h"
#include "Fire.h"
#include <iostream>
#define MAX_TEXTURES 5

Fire::Fire() {

}

Fire::~Fire() {

}

void Fire::Draw() {
    
    glUseProgram(this->shader->program);
    glBindBuffer(GL_ARRAY_BUFFER, this->model->vboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->model->iboId);

    int i = 1;
    for (auto& texture : textures) {
        if (i >= MAX_TEXTURES) break;
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texture.second->idTexture);
        glUniform1i(this->shader->textureUniform[i - 1], i);
        i++;
    }
    glActiveTexture(GL_TEXTURE0);

    if (this->shader->u_DispMax_Uniform != -1)
    {
        glUniform1f(this->shader->u_DispMax_Uniform, u_DispMax);
    }

    if (this->shader->u_Time_Uniform != -1)
    {
        glUniform1f(this->shader->u_Time_Uniform, u_Time);
    }

    sendCommonData();
    glDrawElements(GL_TRIANGLES, this->model->nrIndici, GL_UNSIGNED_SHORT, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Fire::Update(float deltaTime) {
    u_Time = clock() / speedFactor;
}